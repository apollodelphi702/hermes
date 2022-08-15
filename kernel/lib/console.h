#pragma once

/// Typedef for Hermes command.
/// Hermes commands are max 10 args of max 100 chars (1 byte reserved for NULL).
typedef char command_t[10][101];

/**
 * Parse and execute the specified command.
 * @param command The command to parse and execute.
 * @param argc The number of arguments to parse. (This does not include the command itself).
 */
void exec_command(command_t command, int argc);

/**
 * Starts the system's UART console.
 */
_Noreturn void start_uart_console();
