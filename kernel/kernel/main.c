#include <stdint.h>

#include <drivers/mmio.h>
#include <drivers/uart.h>

__attribute__((unused)) _Noreturn
void kernel_main(__attribute__((unused)) uint64_t deviceTree,
                           __attribute__((unused)) uint64_t x1,
                           __attribute__((unused)) uint64_t x2,
                           __attribute__((unused)) uint64_t x3) {

    /* Initialize MMIO for Raspberry Pi 3 */
    mmio_init(3);

    /* Initialize UART */
    uart_init();
    uart_puts("Hello, world!\r\n");

    // Continuously echo any input to the UART.
    while (1) {
        unsigned int c;

        // Read a character and, if it's a carriage return (\r) replace it with a carriage return
        // and line feed (\r\n), otherwise just print the character.
        if ((c = uart_getc()) == '\r') {
            uart_putc('\r');
            uart_putc('\n');
        } else uart_putc(c);
    }

}
