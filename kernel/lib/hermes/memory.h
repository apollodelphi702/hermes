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
 * Alias to memset for readability that sets the value to '\0'.
 *
 * @param base The base address to start clearing from.
 * @param size The size of the buffer to clear.
 */
void memzero(void* base, size_t size);

/**
 * Copies a buffer of the specified size (length in bytes) from source to destination.
 * If destination and source overlap, the behavior is undefined.
 * Applications in which destination and source overlap should use memmove instead.
 *
 * @param destination The destination pointer, to copy to.
 * @param source The source pointer, to copy from.
 * @param size The number of bytes to copy.
 * @return The pointer to the destination.
 */
void* memcpy(void* restrict destination, const void* restrict source, size_t size);

/**
 * Copies a buffer of the specified size (length in bytes) from source to destination.
 * memmove does this in a non-destructive way. The buffers may overlap.
 * If the buffers do not overlap, memcpy should be used for better performance.
 *
 * @param destination The destination pointer, to copy to.
 * @param source The source pointer, to copy from.
 * @param size The number of bytes to copy.
 * @return The pointer to the destination.
 */
void* memmove(void* destination, const void* source, size_t size);
