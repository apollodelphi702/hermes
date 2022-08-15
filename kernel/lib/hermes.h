/** LibHermes **/
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
 * Zeroes out a memory buffer, starting from base to base + size.
 * Enables compiler/CPU optimization by clearing in full system words (64-bit) instead
 * of byte-by-byte.
 *
 * @param base The base address to start clearing from.
 * @param size The size of the buffer to clear.
 */
void memzero(void* base, size_t size);

/**
 * Measures the length of a string by counting up to the first null byte.
 * The returned value is the number of bytes between the string pointer (base address) and the first
 * null byte found after that.
 *
 * @param str A pointer to the string to count the length of.
 * @return The length of the string in bytes.
 */
int strlen(const char* str);

/**
 * Compares str1 to str2, returns the ASCII difference of the first character where the strings differ
 * or 0 if there is no difference.
 * Both strings are compared until a null byte is reached. Alternatively use strncmp to compare strings
 * up to a certain length.
 *
 * @param str1 The base address of the first string to compare.
 * @param str2 The base address of the second string to compare.
 * @return Difference of character where strings diverge or 0 if no difference in strings.
 */
int strcmp(const char* str1, const char* str2);

/**
 * Like strcmp but compares up to a certain length.
 *
 * @see strcmp
 * @param str1 The base address of the first string to compare.
 * @param str2 The base address of the second string to compare.
 * @param n The length in bytes to compare.
 * @return Difference of character where strings diverge or 0 if no difference in strings.
 */
int strncmp(const char* str1, const char* str2, int n);
