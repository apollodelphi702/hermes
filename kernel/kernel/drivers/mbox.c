#include <drivers/mbox.h>

#define MBOX_DATA       0xFFFFFFF0
#define MBOX_CHANNEL    0xF

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

    // Check that the
}
