#include <stdint.h>

#include <drivers/mmio.h>
#include <drivers/uart.h>

#include <console.h>
#include <hermes.h>

__attribute__((unused)) _Noreturn
void kernel_main(__attribute__((unused)) uint64_t deviceTree,
                           __attribute__((unused)) uint64_t x1,
                           __attribute__((unused)) uint64_t x2,
                           __attribute__((unused)) uint64_t x3) {

    /* Initialize MMIO */
    mmio_init();

    /* Initialize UART */
    char buffer[64] = {0};

    // Initialize UART and print banner.
    uart_init();
    uart_clear();
    uart_puts("Welcome to Hermes v0.0.1!\r\n");

    // Print system information
    uart_puts("Raspberry Pi Board Version: ");
    uart_puts(uitoa(buffer, get_mmio_board_type(), 10, 64));
    uart_puts("\r\n");

    memzero(buffer, 64);
    uart_puts("System Current Clock Speed: ");
    uart_puts(uitoa(buffer, get_current_clock_speed(MBOX_TAG_CLOCK_ARM) / 1000000, 10, 64));
    uart_puts(" Mhz\r\n");

    memzero(buffer, 64);
    uart_puts("System Min Clock Speed: ");
    uart_puts(uitoa(buffer, get_min_clock_speed(MBOX_TAG_CLOCK_ARM) / 1000000, 10, 64));
    uart_puts(" Mhz\r\n");

    uint32_t max_clock_speed = get_max_clock_speed(MBOX_TAG_CLOCK_ARM);

    memzero(buffer, 64);
    uart_puts("System Max Clock Speed: ");
    uart_puts(uitoa(buffer, max_clock_speed / 1000000, 10, 64));
    uart_puts(" Mhz\r\n");

    if (!set_current_clock_speed(MBOX_TAG_CLOCK_ARM, max_clock_speed, false)) {
        uart_puts("Failed to set clock speed.\r\n");
    }

    memzero(buffer, 64);
    uart_puts("System Current Clock Speed: ");
    uart_puts(uitoa(buffer, get_current_clock_speed(MBOX_TAG_CLOCK_ARM) / 1000000, 10, 64));
    uart_puts(" Mhz\r\n");

    // Finally, start UART console.
    start_uart_console();

}
