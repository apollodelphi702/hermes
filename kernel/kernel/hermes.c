#include <hermes.h>
#include <stdint.h>

void memset(void* buffer, unsigned char value, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *((uint8_t*) buffer + i) = value;
    }
}

void memzero(void* base, size_t size) {

    if (base == NULL || size == 0) return;

    size_t* baseSz = (size_t*) base;

    size_t clearSize = size / sizeof(size_t);
    size_t* ptr = baseSz;

    // Clear up to the register-sized max
    for (; ptr < baseSz + clearSize; ptr++) {
        *ptr = 0;
    }

    // Clear the remainder using byte operations.
    uint8_t* bytePtr = ((uint8_t*) ptr) + 1;
    for (; bytePtr < (uint8_t*)(baseSz) + size; bytePtr++) {
        *bytePtr = 0;
    }

}

int strlen(const char* str) {
    int len = 0;
    while (str[len] != 0)
        len++;

    return len;
}

int strcmp(const char* str1, const char* str2) {

    // Loop until the end of str1 is reached.
    while (*str1 != 0) {
        // If the current character in str1 does not match the current character in str2, or
        // (implicitly) the end of the second string is reached, break out of the loop early.
        if (*str1 != *str2) break;

        // Increment both pointers to compare the next character.
        str1++;
        str2++;
    }

    // At this point the characters differ, so return the ASCII difference. Each character is
    // 'casted' as unsigned to ensure the math is correct when subtracting ASCII values >= 128.
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;

}

int strncmp(const char* str1, const char* str2, int n) {

    int index = 0;

    // Loop until the end of str1 is reached.
    while (*str1 != 0 && index < n - 1) {
        // If the current character in str1 does not match the current character in str2, or
        // (implicitly) the end of the second string is reached, break out of the loop early.
        if (*str1 != *str2) break;

        // Increment both pointers to compare the next character.
        str1++;
        str2++;
        index++;
    }

    // At this point the characters differ, so return the ASCII difference. Each character is
    // 'casted' as unsigned to ensure the math is correct when subtracting ASCII values >= 128.
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;

}
