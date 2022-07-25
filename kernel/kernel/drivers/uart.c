#include <drivers/uart.h>
#include <drivers/mmio.h>

#include <stddef.h>

#include <debug.h>

void uart_init() {

    // Disable UART0.
    mmio_write(UART0_CR, 0);

    // For the Raspberry Pi 3 and 4 the UART_CLOCK is system-clock
    // dependent by default.
    // Thus, we set it to 4Mhz and use that to consistently set the baud
    // rate.
    if (get_mmio_board_type() >= 3) {
        // A Mailbox message with set clock rate to 4MHz.
        mbox[0] = 9 * 4;
        mbox[1] = 0;
        mbox[2] = 0x38002;
        mbox[3] = 12;
        mbox[4] = 8;
        mbox[5] = 2;
        mbox[6] = 4000000;
        mbox[7] = 0;
        mbox[8] = 0;

        // UART_CLOCK = 3,000,000.
        unsigned int r = (((unsigned int)((unsigned long) &mbox) & ~0xF) | 8);
        while (mmio_read(MBOX_STATUS) & 0x80000000);
        // Send message to property channel and wait for response.
        mmio_write(MBOX_WRITE, r);
        while ( (mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r );
    }

    // Map UART0 to the GPIO pins. (It's normally mapped to the bluetooth adapter).
    unsigned int r = mmio_read(GPFSEL1);
    r &= ~((7 << 12) | (7 << 15));  // pins 14 and 15
    r |= (4 << 12) | (4 << 15);     // alt0
    mmio_write(GPFSEL1, r);

    // Enable pins 14 and 15.
    mmio_write(GPPUD, 0);
    delay(150);
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));           // Enable pins 14 and 15.
    delay(150);
    mmio_write(GPPUDCLK0, 0);                               // Flush GPIO configuration.

    // Clear pending interrupts.
    mmio_write(UART0_ICR, 0x7FF);

    // Set baud rate = 115200.
    // We do this by setting the integer and fractional parts of the
    // baud rate value.

    // Interval Baud Divider - 115200 baud
    mmio_write(UART0_IBRD, 2);
    // Fractional part register
    mmio_write(UART0_FBRD, 0xB);

    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts.
//    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
//                           (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));

}

unsigned char uart_getc() {
    while (mmio_read(UART0_FR) & (1 << 4));
    return mmio_read(UART0_DR);
}

void uart_putc(unsigned char c) {
    // Wait for UART to become ready to transmit.
    while (mmio_read(UART0_FR) & (1 << 5));
    mmio_write(UART0_DR, c);
}

void uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        uart_putc((unsigned char) str[i]);
    }
}
