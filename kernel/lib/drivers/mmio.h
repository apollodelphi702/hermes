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

// TODO: refactor board type to be system-wide (rather than just MMIO).
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

    // Mailbox

    /// The base address for Mailbox MMIO registers.
    MBOX_BASE    = 0xB880,
    /// Mailbox Read Register
    MBOX_READ    = (MBOX_BASE + 0x00),
    /// Mailbox Poll Register
    MBOX_POLL    = (MBOX_BASE + 0x10),
    /// Mailbox Sender Register
    MBOX_SENDER  = (MBOX_BASE + 0x14),
    /// Mailbox Status Register
    MBOX_STATUS  = (MBOX_BASE + 0x18),
    /// Mailbox Config Register
    MBOX_CONFIG  = (MBOX_BASE + 0x1C),
    /// Mailbox Write Register
    MBOX_WRITE   = (MBOX_BASE + 0x20),


    // GPIO

    /// The base address for GPIO MMIO registers.
    GPIO_BASE   = 0x200000,
    /// GPIO Function Select 0
    GPFSEL0     = (GPIO_BASE + 0x00),
    /// GPIO Function Select 1
    GPFSEL1     = (GPIO_BASE + 0x04),
    /// GPIO Function Select 2
    GPFSEL2     = (GPIO_BASE + 0x08),
    /// GPIO Function Select 3
    GPFSEL3     = (GPIO_BASE + 0x0C),
    /// GPIO Function Select 4
    GPFSEL4     = (GPIO_BASE + 0x10),
    /// GPIO Function Select 5
    GPFSEL5     = (GPIO_BASE + 0x14),
    /// GPIO Pin Output Set 0
    GPSET0      = (GPIO_BASE + 0x1C),
    /// GPIO Pin Output Set 1
    GPSET1      = (GPIO_BASE + 0x20),
    /// GPIO Pin Output Clear 0
    GPCLR0      = (GPIO_BASE + 0x28),
    /// GPIO Pin Output Clear 1
    GPCLR1      = (GPIO_BASE + 0x2C),
    /// GPIO Pin Pull-up/down Enable
    GPPUD       = (GPIO_BASE + 0x94),
    /// GPIO Pin Pull-up/down Enable Clock 0
    GPPUDCLK0   = (GPIO_BASE + 0x98),
    /// GPIO Pin Pull-up/down Enable Clock 1
    GPPUDCLK1   = (GPIO_BASE + 0x9C),


    // UART

    /// The base address for UART MMIO registers.
    UART0_BASE   = 0x201000,
    /// UART Data Register
    UART0_DR     = (UART0_BASE + 0x00),
    /// UART Receive Status / Error Clear Register
    UART0_RSRECR = (UART0_BASE + 0x04),
    /// UART Flag Register
    UART0_FR     = (UART0_BASE + 0x18),
    /// UART Integer Baud Rate Divisor
    UART0_IBRD   = (UART0_BASE + 0x24),
    /// UART Fractional Baud Rate Divisor
    UART0_FBRD   = (UART0_BASE + 0x28),
    /// UART Line Control Register
    UART0_LCRH   = (UART0_BASE + 0x2C),
    /// UART Control Register
    UART0_CR     = (UART0_BASE + 0x30),
    /// UART Interrupt FIFO Level Select Register
    UART0_IFLS   = (UART0_BASE + 0x34),
    /// UART Interrupt Mask Set/Clear Register
    UART0_IMSC   = (UART0_BASE + 0x38),
    /// UART Raw Interrupt Status Register
    UART0_RIS    = (UART0_BASE + 0x3C),
    /// UART Masked Interrupt Status Register
    UART0_MIS    = (UART0_BASE + 0x40),
    /// UART Interrupt Clear Register
    UART0_ICR    = (UART0_BASE + 0x44),
    /// UART DMA Control Register
    UART0_DMACR  = (UART0_BASE + 0x48),

};
