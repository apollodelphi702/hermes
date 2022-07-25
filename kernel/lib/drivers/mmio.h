#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Mailbox Message buffer.
 */
extern volatile unsigned int mbox[36];

/**
 * Initializes the Memory-Mapped IO (MMIO) driver.
 * If the specified boardType is not recognized, MMIO will not be initialized.
 *
 * @param boardType The numerical board type. (e.g., 2 for Raspberry Pi 2, etc.,)
 */
void mmio_init(unsigned int boardType);

/**
 * Trivial alias to determine whether the MMIO driver has been initialized.
 * @return True if the driver is initialized, otherwise false.
 */
bool is_mmio_initialized();

// TODO: refactor this to be system-wide.
/**
 * Trivial alias to return the current board type that the MMIO driver is using.
 * @return The numeric board type that the MMIO driver is using.
 */
unsigned int get_mmio_board_type();

/**
 * Reads the value from the specified MMIO register and returns it
 * as an unsigned 32-bit integer.
 * @param offset The MMIO register offset to read from.
 * @return The value held in that register offset.
 */
uint32_t mmio_read(uint64_t offset);

/**
 * Writes the specified unsigned 32-bit value into the specified MMIO
 * register.
 * @param offset The MMIO register offset to write to.
 * @param data The value to write into the register.
 */
void mmio_write(uint64_t offset, uint32_t value);



/// MMIO register offsets.
enum {

    // GPIO

    /// The base address for GPIO registers.
    GPIO_BASE = 0x200000,
    GPFSEL0 = GPIO_BASE,
    GPFSEL1 = (GPIO_BASE + 0x04),
    GPFSEL2 = (GPIO_BASE + 0x08),
    GPFSEL3 = (GPIO_BASE + 0x0C),
    GPFSEL4 = (GPIO_BASE + 0x10),
    GPFSEL5 = (GPIO_BASE + 0x14),
    GPPUD = (GPIO_BASE + 0x94), // Controls pulling all GPIO pins up/down.
    GPPUDCLK0 = (GPIO_BASE + 0x98), // Controls pulling a specific GPIO pin up/down.


    // UART

    /// The base address for UART registers.
    UART0_BASE = (GPIO_BASE + 0x1000),
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),


    // Mailbox

    /// The base address for Mailbox registers.
    MBOX_BASE = 0xB880,
    MBOX_READ    = (MBOX_BASE + 0x00),
    MBOX_STATUS  = (MBOX_BASE + 0x18),
    MBOX_WRITE   = (MBOX_BASE + 0x20)

};
