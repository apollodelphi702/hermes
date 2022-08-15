#pragma once

#include <stddef.h>

/**
 * Sets every byte in a buffer, starting at the pointer base, to base + size.
 * @param base The starting pointer to begin writing bytes.
 * @param value The value to write into each byte.
 * @param size The size of the buffer (number of bytes) to write.
 */
void memset(void* base, unsigned char value, size_t size);

/**
 * Swaps the value stored at address a with the value stored at address b.
 * @param a Value to swap with *b.
 * @param b Value to swap with *a.
 */
void memswap(char* a, char* b);

/**
 * Zeroes out a memory buffer, starting from base to base + size.
 * Enables compiler/CPU optimization by clearing in full system words (64-bit) instead
 * of byte-by-byte.
 *
 * @param base The base address to start clearing from.
 * @param size The size of the buffer to clear.
 */
void memzero(void* base, size_t size);
