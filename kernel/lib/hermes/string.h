#pragma once

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

/**
 * Reverses the specified string.
 * @param str The string to reverse.
 * @param length The length of the string (not including null terminators).
 */
void strrev(char* str, unsigned int length);
