#include <hermes/memory.h>

#include <stdint.h>

void memset(void* buffer, unsigned char value, size_t size) {
    for (size_t i = 0; i < size; i++) {
        *((uint8_t*) buffer + i) = value;
    }
}

void memswap(char* a, char* b) {
    uint8_t temp = *(uint8_t*)b;
    *(uint8_t*)b = *(uint8_t*)a;
    *(uint8_t*)a = temp;
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
