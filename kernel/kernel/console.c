#include <console.h>

#include <hermes.h>
#include <drivers/uart.h>

#define COMMAND_NOT_IMPLEMENTED(cmd) uart_puts(cmd ": not implemented!\r\n");

void exec_command(command_t command) {
    const char* command_name = command[0];

    if          (strcmp(command_name, "cd") == 0) {
        COMMAND_NOT_IMPLEMENTED("cd");
        return;
    } else if   (strcmp(command_name, "ls") == 0) {
        COMMAND_NOT_IMPLEMENTED("ls");
        return;
    }

    uart_puts("\033[1;31mInvalid command!\033[0;39m\r\n");
}

_Noreturn void start_uart_console() {

    // Zero-initialize command array.
    command_t command = {0};

    // Initialize counters.
    int wordCounter = 0, charCounter = 0;

    // Continuously read and parse UART input.
    while (1) {
        unsigned int c;

        // Read a character and, if it's a carriage return (\r) replace it with a carriage return
        // and line feed (\r\n), otherwise just print the character.
        if ((c = uart_getc()) == '\r') {
            uart_putc('\r');
            uart_putc('\n');

            command[wordCounter][charCounter + 1] = '\0';
            wordCounter = 0;
            charCounter = 0;

            exec_command(command);
        }
        // If the character is a space, move the counter to the next word in the array.
        else if (c == ' ') {
            // Stop listening if the limits are exceeded.
            if (wordCounter >= 10) continue;

            // Echo the character and update counters.
            command[wordCounter][charCounter + 1] = '\0';
            uart_putc(c);
            wordCounter += 1;
            charCounter = 0;
        }
        // Otherwise, the letter is echoed and the input is saved as part of the command.
        else {
            // Stop listening if the limits are exceeded.
            if (charCounter >= 100) continue;

            // Echo the character, store the character and update the counter.
            uart_putc(c);
            command[wordCounter][charCounter] = c;
            charCounter += 1;
        }

    }

}
