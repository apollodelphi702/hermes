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
    memset(base, '\0', size);
}

void* memcpy(void* restrict destination, const void* restrict source, size_t size) {
    const uint8_t* restrict byte_source = (const uint8_t* restrict) source;
    uint8_t* restrict byte_destination = (uint8_t* restrict) destination;

    for (size_t i = 0; i < size; i++) byte_destination[i] = byte_source[i];
    return destination;
}

/* memmove.c -- copy memory.
   Copy LENGTH bytes from SOURCE to DEST.  Does not null-terminate.
   In the public domain.
   By David MacKenzie <djm@gnu.ai.mit.edu>.  */
void* memmove(void* destination, const void* source, size_t size) {
    const uint8_t* byte_source = source;
    uint8_t* byte_destination = destination;

    if (byte_source < byte_destination)
        /* Moving from low memory to high memory, so start at the end. */
        for (byte_source += size, byte_destination += size; size; --size)
            *--byte_destination = *--byte_source;
    else if (byte_source != byte_destination)
        /* Moving from high memory to low memory, so start at the beginning. */
        for (; size; --size)
            *byte_destination++ = *byte_source++;

    return destination;
}
