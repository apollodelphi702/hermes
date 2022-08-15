#include <stdint.h>

#include <drivers/mmio.h>
#include <drivers/uart.h>

#include <console.h>

__attribute__((unused)) _Noreturn
void kernel_main(__attribute__((unused)) uint64_t deviceTree,
                           __attribute__((unused)) uint64_t x1,
                           __attribute__((unused)) uint64_t x2,
                           __attribute__((unused)) uint64_t x3) {

    /* Initialize MMIO for Raspberry Pi 3 */
    mmio_init(3);

    /* Initialize UART */
    uart_init();
    uart_puts("Welcome to Hermes v0.0.1!\r\n");
    start_uart_console();

}
