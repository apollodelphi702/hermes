# pragma once

void uart_init();

unsigned char uart_getc();

void uart_putc(unsigned char c);

void uart_puts(const char* str);

void uart_clear();
