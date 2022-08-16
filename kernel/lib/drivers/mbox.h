#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <drivers/mmio.h>

/// Flag that denotes a mailbox request.
#define MBOX_REQUEST    0x0
/// Flag that denotes a mailbox response.
#define MBOX_RESPONSE   0x80000000

/// Flag that denotes an empty mailbox.
#define MBOX_EMPTY      0x40000000
/// Flag that denotes a full mailbox.
#define MBOX_FULL       0x80000000

/// Mask for the Mailbox message data.
#define MBOX_DATA       0xFFFFFFF0
/// Mask for Mailbox message channel.
#define MBOX_CHANNEL    0xF

typedef enum {
    /// Channel for requests from the ARM CPU to VideoCore.
    MBOX_CHANNEL_ARM_TO_VC          = 0x8,
} mbox_channel_t;

typedef enum {
    MBOX_TAG_STATE_REQUEST          = 0x0,
    MBOX_TAG_STATE_RESPONSE         = 0x1,
} mbox_tag_state_t;

typedef enum {
    /// Tag for end of message.
    MBOX_TAG_END                    = 0x0,

    /* Video-core */
    MBOX_TAG_GET_FIRMWARE_VERSION   = 0x1,

    /* Hardware */
    MBOX_TAG_GET_BOARD_MODEL        = 0x10001,
    MBOX_TAG_GET_BOARD_REVISION     = 0x10002,
    MBOX_TAG_GET_BOARD_MAC_ADDRESS  = 0x10003,
    MBOX_TAG_GET_BOARD_SERIAL       = 0x10004,
    MBOX_TAG_GET_ARM_MEMORY         = 0x10005,
    MBOX_TAG_GET_VC_MEMORY          = 0x10006,
    MBOX_TAG_GET_CLOCKS             = 0x10007,

    /* Clocks */
    MBOX_TAG_GET_CLOCK_STATE        = 0x30001,
    MBOX_TAG_SET_CLOCK_STATE        = 0x38001,
    MBOX_TAG_GET_CLOCK_RATE         = 0x30002,
    MBOX_TAG_SET_CLOCK_RATE         = 0x38002,
    MBOX_TAG_GET_MAX_CLOCK_RATE     = 0x30004,
    MBOX_TAG_GET_MIN_CLOCK_RATE     = 0x30007,
    MBOX_TAG_GET_TURBO              = 0x30009,
    MBOX_TAG_SET_TURBO              = 0x38009,

    /* Voltage */
    MBOX_TAG_GET_VOLTAGE            = 0x30003,
    MBOX_TAG_SET_VOLTAGE            = 0x38003,
    MBOX_TAG_GET_TEMPERATURE        = 0x30006,
    MBOX_TAG_GET_MAX_TEMPERATURE    = 0x3000A,

    /* Framebuffer */
    MBOX_TAG_ALLOCATE_BUFFER        = 0x40001,
    MBOX_TAG_RELEASE_BUFFER         = 0x48001,
    MBOX_TAG_BLANK_SCREEN           = 0x40002,
    MBOX_TAG_GET_PHYSICAL_SIZE      = 0x40003,
    MBOX_TAG_TEST_PHYSICAL_SIZE     = 0x44003,
    MBOX_TAG_SET_PHYSICAL_SIZE      = 0x48003,
    MBOX_TAG_GET_VIRTUAL_SIZE       = 0x40004,
    MBOX_TAG_TEST_VIRTUAL_SIZE      = 0x44004,
    MBOX_TAG_SET_VIRTUAL_SIZE       = 0x48004,
    MBOX_TAG_GET_DEPTH              = 0x40005,
    MBOX_TAG_TEST_DEPTH             = 0x44005,
    MBOX_TAG_SET_DEPTH              = 0x48005,
    MBOX_TAG_GET_PIXEL_ORDER        = 0x40006,
    MBOX_TAG_TEST_PIXEL_ORDER       = 0x44006,
    MBOX_TAG_SET_PIXEL_ORDER        = 0x48006,
    MBOX_TAG_GET_ALPHA_MODE         = 0x40007,
    MBOX_TAG_TEST_ALPHA_MODE        = 0x44007,
    MBOX_TAG_SET_ALPHA_MODE         = 0x48007,
    MBOX_TAG_GET_PITCH              = 0x40008,
    MBOX_TAG_GET_VIRTUAL_OFFSET     = 0x40009,
    MBOX_TAG_TEST_VIRTUAL_OFFSET    = 0x44009,
    MBOX_TAG_SET_VIRTUAL_OFFSET     = 0x48009,
    MBOX_TAG_GET_OVERSCAN           = 0x4000A,
    MBOX_TAG_TEST_OVERSCAN          = 0x4400A,
    MBOX_TAG_SET_OVERSCAN           = 0x4800A,
    MBOX_TAG_GET_PALETTE            = 0x4000B,
    MBOX_TAG_TEST_PALETTE           = 0x4400B,
    MBOX_TAG_SET_PALETTE            = 0x4800B,
    MBOX_TAG_SET_CURSOR_INFO        = 0x8011,
    MBOX_TAG_SET_CURSOR_STATE       = 0x8010
} mbox_tag_t;

typedef enum {
    MBOX_TAG_CLOCK_RESERVED         = 0x0,
    MBOX_TAG_CLOCK_EMMC             = 0x1,
    MBOX_TAG_CLOCK_UART             = 0x2,
    MBOX_TAG_CLOCK_ARM              = 0x3,
    MBOX_TAG_CLOCK_CORE             = 0x4,
    MBOX_TAG_CLOCK_V3D              = 0x5,
    MBOX_TAG_CLOCK_H264             = 0x6,
    MBOX_TAG_CLOCK_ISP              = 0x7,
    MBOX_TAG_CLOCK_SDRAM            = 0x8,
    MBOX_TAG_CLOCK_PIXEL            = 0x9,
    MBOX_TAG_CLOCK_PWM              = 0xA
} mbox_tag_clock_t;

typedef struct {
    mbox_tag_t tag;
    uint32_t byte_length;
    union {
        uint8_t value_u8;
        uint32_t value_u32;
        uint8_t buffer_u8[256];
        uint32_t buffer_u32[64];
    } data;
} mbox_tag_value_t;


// TODO: use typedef struct for mailbox messages?

#define __VA_ARGC__(...)  (sizeof((uint8_t[]){__VA_ARGS__})) // NOLINT(bugprone-reserved-identifier)

/**
 * Macro to define a mailbox message of the required type and define the parameters inline.
 * @param name The object name the message should be created with.
 */
#define DefineMailboxMessage(name, ...) volatile uint32_t __attribute__((aligned(16))) \
                                        (mbox_msg_ ## name)[(__VA_ARGC__(__VA_ARGS__))] = {__VA_ARGS__}

#define DefineMailboxRequest(name, ...) volatile uint32_t __attribute__((aligned(16))) \
                                        (mbox_msg_ ## name)[__VA_ARGC__(__VA_ARGS__) + 3] = {   \
                                        ((__VA_ARGC__(__VA_ARGS__) + 3) * sizeof(uint32_t)),    \
                                        MBOX_REQUEST,                                           \
                                        __VA_ARGS__,                                            \
                                        MBOX_TAG_END}

/**
 * Readability alias to wait while the mailbox status matches the specified status (i.e., wait until that status is no
 * longer set).
 */
#define mbox_wait_while(status)     do {} while (mmio_read(MBOX_STATUS) & status)

/**
 * Readability alias to wait until the mailbox status matches the specified status.
 */
#define mbox_wait_until(status)     do {} while (!(mmio_read(MBOX_STATUS) & status))

/**
 * Send the specified message pointer to the specified mailbox channel when the mailbox becomes available.
 * You can use the DefineMailboxMessage macro to make a buffer and pre-populate it with a specific message payload.
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
 * Initializes a message request. This includes zeroing out tag values (to be filled) and setting the tag
 * request/response value to request.
 *
 * Returns a boolean to indicate whether the message was successfully initialized. If it wasn't, it is likely due to
 * an incorrect buffer size or the message not being 16-byte aligned in memory.
 *
 * @param message The message to initialize.
 * @return Whether the message was successfully initialized.
 */
bool mbox_init(volatile uint32_t* message);

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

/**
 * Gets the tag value from the specified mailbox message.
 *
 * @param message The message to get the tag value from.
 * @param tag The tag to get the value of.
 * @param value A pointer to the structure to store the value in.
 * @return True if successful, otherwise false.
 */
bool mbox_get_tag_value(volatile uint32_t* message, mbox_tag_t tag, mbox_tag_value_t* value);

/**
 * Sets the specified tag value in the specified mailbox message.
 * The message must have the required number of bytes defined in it to hold the value
 * being set, otherwise the function will fail.
 *
 * @param message The message to write the tag value into.
 * @param offset The offset (from the first value) to start writing the value data buffer at.
 * @param value The value (which contains the tag for which it belongs) to write into the message.
 * @return True if successful, otherwise false.
 */
bool mbox_set_tag_value(volatile uint32_t* message, uint32_t offset, mbox_tag_value_t* value);

uint8_t mbox_get_tag_value_u8(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset);

bool mbox_set_tag_value_u8(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset, uint8_t value);

uint32_t mbox_get_tag_value_u32(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset);

bool mbox_set_tag_value_u32(volatile uint32_t* message, mbox_tag_t tag, uint32_t offset, uint32_t value);
