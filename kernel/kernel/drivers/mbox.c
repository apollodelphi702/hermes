#include <drivers/mbox.h>
#include <hermes/memory.h>
#include <hermes/utilities.h>

#define ptr_to_uint32(x) ((uint32_t) (uint64_t) x)

void mbox_write(uint8_t channel, const volatile uint32_t* message) {
    // Ensure the message is 16-byte aligned.
    // TODO: exception handling?
    if ((ptr_to_uint32(message) & MBOX_DATA) != ptr_to_uint32(message)) return;

    // Wait until the mailbox has room for the message.
    mbox_wait_while(MBOX_FULL);

    // Write the message into the mailbox.
    mmio_write(
        MBOX_WRITE,
        (uint32_t) ((uint64_t) message & MBOX_DATA) | ((uint32_t) channel & MBOX_CHANNEL)
    );
}

const volatile uint32_t* mbox_read(uint8_t channel, const volatile uint32_t* filter) {
    // Ensure the filter is 16-byte aligned.
    if ((ptr_to_uint32(filter) & MBOX_DATA) != ptr_to_uint32(filter)) return 0;

    uint32_t message_data;
    uint8_t message_channel;

    do {
        // Wait until there's a value in the mailbox (i.e., it's no longer empty).
        mbox_wait_while(MBOX_EMPTY);

        // Read the message in the mailbox.
        uint32_t message = mmio_read(MBOX_READ);

        // Get the first four bits of the message (the channel).
        message_channel     = message & MBOX_CHANNEL;

        // Get the remaining bits (pointer to the message data).
        message_data        = message & MBOX_DATA;

    }

    // If the above message wasn't for us (i.e., the filter didn't line up) continue
    // polling until the message is for us. This won't do anything unless mailbox_writes
    // and mailbox_reads are interleaved.
    while (message_channel != channel || (filter != 0 && message_data != ptr_to_uint32(filter)));

    // If we're here the message is for us, so return the pointer to the message.
    // Note that we need to cast the value to unsigned 64-bit first because pointers
    // in AArch64 are, naturally, 64-bit.
    return (volatile uint32_t *) (uint64_t) message_data;
}

bool mbox_init(volatile uint32_t* message) {
    // Ensure the message is 16-byte aligned. Otherwise, return early with an error.
    // We do this manually here as mbox_write currently just exits silently.
    if ((ptr_to_uint32(message) & MBOX_DATA) != ptr_to_uint32(message)) return false;

    // If the message size is less than 2, return an error.
    // The message size must be correctly set to indicate that an adequate buffer has been allocated
    // at build time.
    if (message[0] < 2) return false;

    // Set the message mode to request.
    message[1] = MBOX_REQUEST;

    uint32_t index = 2;
    while (index < message[0]) { // Loop until the message is processed entirely.

        // If the current tag is TAG_END, stop processing.
        if (message[index] == MBOX_TAG_END) break;

        // Skip tag name, size and request type.
        uint32_t tag_size = message[index + 2] & 0x3fffffff;
        index += 2;
        message[index++] = MBOX_TAG_STATE_REQUEST;

        // Check that the tag size is valid.
        // (Return invalid if the tag size exceeds the message size).
        if (tag_size > (message[0] - index)) return false;

        // Zero out the tag values (they can be set after initialization).
        memzero((void*) &message[index], tag_size);
        index += tag_size;

    }

    // Indicate success.
    return true;
}

bool mbox_call(uint8_t channel, volatile uint32_t* message) {
    // Ensure the message is 16-byte aligned. Otherwise, return early with an error.
    // We do this manually here as mbox_write currently just exits silently.
    if ((ptr_to_uint32(message) & MBOX_DATA) != ptr_to_uint32(message)) return false;

    // Write the message to the mbox channel.
    mbox_write(channel, message);

    // Wait for a response to the channel to the initial message.
    mbox_read(channel, message);

    // Return whether the call was successful.
    return message[1] == MBOX_RESPONSE;
}

bool mbox_get_tag_value(volatile uint32_t* message, mbox_tag_t tag, mbox_tag_value_t* value) {
    // Ensure the message is 16-byte aligned. Otherwise, return early with an error.
    // We do this manually here as mbox_write currently just exits silently.
    if ((ptr_to_uint32(message) & MBOX_DATA) != ptr_to_uint32(message)) return false;

    // Ensure tag is not an invalid value.
    if (tag == MBOX_TAG_END) return false;

    uint32_t index = 2; // Skip over buffer length and message request/response state.
    while (index < message[0]) { // Loop until the message is read entirely (or until the tag is found).

        if (message[index] == tag) {
            // Tag was found, copy data and return.

            value->tag          = tag;                              // Tag
            value->byte_length  = message[index + 2] & 0x3fffffff;  // Response Tag Size (bytes)
            memcpy(value->data.buffer_u8, (void*) &message[index + 3], value->byte_length);

            return true;
        }

        // Otherwise skip to the next tag.
        index += (
            message[index + 1]  /* Value after tag name is tag size (bytes) */
            >> 2                /* Divide by 4 (we're iterating over uint32 = 4 bytes) */
        ) + 3;                  /* Add 3 (current tag, tag size and tag status) */

    }

    // If the tag wasn't found, return false.
    return false;
}

bool mbox_set_tag_value(volatile uint32_t* message, uint32_t offset, mbox_tag_value_t* value) {
    // Ensure the message is 16-byte aligned. Otherwise, return early with an error.
    // We do this manually here as mbox_write currently just exits silently.
    if ((ptr_to_uint32(message) & MBOX_DATA) != ptr_to_uint32(message)) return false;

    // Ensure tag is not an invalid value.
    if (value->tag == MBOX_TAG_END) return false;

    uint32_t index = 2; // Skip over buffer length and message request/response state.
    while (index < message[0]) { // Loop until the message is read entirely (or until the tag is found).

        if (message[index] == value->tag) {
            // Tag was found, copy data and return.

            // If the message request length is less than the value, fail.
            if (message[index + 1] < value->byte_length) return false;

            // Otherwise, write the value in with memcpy.
            memcpy((uint8_t*) &(message[index + 3]) + offset, value->data.buffer_u8, value->byte_length);

            return true;
        }

        // Otherwise skip to the next tag.
        index += (
             message[index + 1]  /* Value after tag name is tag size (bytes) */
             >> 2                /* Divide by 4 (we're iterating over uint32 = 4 bytes) */
         ) + 3;                  /* Add 3 (current tag, tag size and tag status) */

    }

    // If the tag wasn't found, return false.
    return false;
}

uint8_t mbox_get_tag_value_u8(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset) {
    mbox_tag_value_t value;
    mbox_get_tag_value(message, tag, &value);
    return value.data.buffer_u8[offset];
}

bool mbox_set_tag_value_u8(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset, uint8_t value) {
    mbox_tag_value_t tagValue = { .byte_length = sizeof(uint32_t), .tag = tag, .data.value_u8 = value };
    return mbox_set_tag_value(message, offset, &tagValue);
}

uint32_t mbox_get_tag_value_u32(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset) {
    mbox_tag_value_t value;
    mbox_get_tag_value(message, tag, &value);
    return value.data.buffer_u32[offset];
}

bool mbox_set_tag_value_u32(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset, uint32_t value) {
    mbox_tag_value_t tagValue = { .byte_length = sizeof(uint32_t), .tag = tag, .data.value_u32 = value };
    return mbox_set_tag_value(message, offset, &tagValue);
}
