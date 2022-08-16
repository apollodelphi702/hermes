#include <drivers/mbox.h>
#include <hermes/memory.h>
#include "hermes/utilities.h"

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

uint8_t mbox_get_tag_value_u8(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset) {
    mbox_tag_value_t value;
    mbox_get_tag_value(message, tag, &value);
    return value.data.buffer_u8[offset];
}

uint32_t mbox_get_tag_value_u32(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset) {
    mbox_tag_value_t value;
    mbox_get_tag_value(message, tag, &value);
    return value.data.buffer_u32[offset];
}
