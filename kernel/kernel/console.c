#include <console.h>

#include <hermes.h>
#include <drivers/uart.h>

#define DEBUG_PRINT_COMMAND_ARGUMENTS 0

#define COMMAND_NOT_IMPLEMENTED(cmd) uart_puts(cmd ": not implemented!\r\n");

void exec_command(command_t command, int argc) {
    const char* command_name = command[0];

    // If the command is empty, skip processing and wait for the next one.
    if (strlen(command_name) == 0) return;

#if DEBUG_PRINT_COMMAND_ARGUMENTS
    for (int i = 0; i < argc; i++) {
        uart_puts(command[i + 1]);
        uart_puts("\r\n");
    }
#endif

    // Match recognized commands:

    if          (strcmp(command_name, "cd") == 0) {
        COMMAND_NOT_IMPLEMENTED("cd");
        return;
    } else if   (strcmp(command_name, "ls") == 0) {
        COMMAND_NOT_IMPLEMENTED("ls");
        return;
    }

    // If no commands could be matched, print 'invalid command'.

    uart_puts("\033[1;31mInvalid command: ");
    uart_puts(command_name);
    uart_puts("\033[0;39m\r\n");
}

_Noreturn void start_uart_console() {

    // Zero-initialize command array.
    command_t command = {0};

    // Initialize counters.
    int wordCounter = 0, charCounter = 0;

    // Print prompt.
    uart_putc('>');
    uart_putc(' ');

    // Continuously read and parse UART input.
    while (1) {
        unsigned int c;

        // Read a character and, if it's a carriage return (\r) replace it with a carriage return
        // and line feed (\r\n), otherwise just print the character.
        if ((c = uart_getc()) == '\r') {
            uart_putc('\r');
            uart_putc('\n');

            command[wordCounter][charCounter + 1] = '\0';
            int argc = wordCounter;

            wordCounter = 0;
            charCounter = 0;

            exec_command(command, argc);

            // Clear command buffer and print prompt.
            for (size_t i = 0; i < 10; i++) memzero(command[i], 101);
            uart_putc('>');
            uart_putc(' ');
        }
        // If the character is a space, move the counter to the next word in the array.
        else if (c == ' ') {
            // Stop listening if the limits are exceeded.
            if (wordCounter >= 10) continue;

            // Echo the character and update counters.
            command[wordCounter][charCounter] = '\0';
            uart_putc(c);
            wordCounter += 1;
            charCounter = 0;
        }
        // If the character is a backspace, remove a character from the current word (assuming
        // it's not the start of the word).
        else if (c == 0x08 || c == 0x7F) {
            // If we're at the beginning of the line, ignore.
            if (wordCounter < 1 && charCounter < 1) continue;

            // If we're at the start of the word, delete the word and go back to the start of the
            // previous word.
            if (charCounter < 1) {
                // Zero out the current word.
                memzero(command[wordCounter], 101);

                // Go back to the previous word.
                wordCounter--;
                charCounter = strlen(command[wordCounter]);
            }
            // Otherwise, delete the current character.
            else {
                // Set the current character to null.
                command[wordCounter][charCounter - 1] = '\0';

                // Go to the previous character.
                charCounter--;
            }

            // Print a backspace (move back, print delete character, move back).
            uart_putc(0x08);
            uart_putc(0x7F);
            uart_putc(0x08);
        }
        // Prevent navigation with arrow keys
        else if (c == 0x1b) continue;
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
