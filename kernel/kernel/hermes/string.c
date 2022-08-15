#include <hermes/string.h>
#include <hermes/memory.h>

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

void strrev(char* str, unsigned int length) {
    unsigned int start = 0, end = length - 1;
    while(start < end) {
        memswap(str + start, str + end);
        start++; end--;
    }
}
