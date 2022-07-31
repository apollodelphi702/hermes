#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "mmio.h"

/// Flag that denotes a mailbox request.
#define MBOX_REQUEST    0x0
/// Flag that denotes a mailbox response.
#define MBOX_RESPONSE   0x80000000

/// Flag that denotes an empty mailbox.
#define MBOX_EMPTY      0x40000000
/// Flag that denotes a full mailbox.
#define MBOX_FULL       0x80000000

/**
 * Macro to create a mailbox message of the required type and define the parameters inline.
 * @param name The object name the message should be created with.
 */
#define CreateMailboxMessage(name, ...) volatile uint32_t __attribute__((aligned(16))) name[36] = {__VA_ARGS__}

/**
 * Readability alias to wait while the mailbox status matches the specified status (i.e., wait until that status is no
 * longer set).
 */
#define mbox_wait_while(status) do {} while (mmio_read(MBOX_STATUS) & status)

/**
 * Readability alias to wait until the mailbox status matches the specified status.
 */
#define mbox_wait_until(status) do {} while (!(mmio_read(MBOX_STATUS) & status))

/**
 * Send the specified message pointer to the specified mailbox channel when the mailbox becomes available.
 * You can use the CreateMailboxMessage macro to make a buffer and pre-populate it with a specific message payload.
 *
 * If the message pointer is not 16-byte aligned, this function does nothing.
 *
 * @param channel The channel to make the call to.
 * @param message The pointer to the message to write to the mailbox. Must be 16-byte aligned.
 */
void mbox_write(uint8_t channel, const volatile uint32_t* message);

/**
 * Waits for the specified mailbox channel to have a message written to it and returns a pointer to the message. If
 * this message is a response to an mbox_write() then the returned pointer will be the same address as the message
 * parameter that was used with the corresponding mbox_write() call.
 *
 * If the filter is specified, this function will only return when the response address matches the filter.
 * The filter must be 16-byte aligned.
 *
 * If the mailbox could not be read from, or the filter is not 16-byte aligned, this function will return a null
 * pointer to indicate failure.
 *
 * @param channel The channel to retrieve data from.
 * @param filter Read only a mailbox response to this address.
 * @return A pointer to the mailbox data.
 */
const volatile uint32_t* mbox_read(uint8_t channel, const volatile uint32_t* filter);

/**
 * Makes a call to a mailbox. (Writes a message to the mailbox and reads the response).
 * This can be considered a higher level abstraction over mbox_write() and mbox_read(), specifically for mailbox calls
 * where a response is expected but doesn't necessarily need to be read.
 *
 * @param channel The channel to make the call to.
 * @param message The message to invoke the call with.
 * @return True on success, otherwise false.
 */
bool mbox_call(uint8_t channel, volatile uint32_t* message);
