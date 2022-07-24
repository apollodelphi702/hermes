#pragma once

#include <stdint.h>

/**
 * Loop for n = count times.
 * This is done in such a way that the compiler won't optimize it out.
 * @param count The number of times to loop.
 */
static inline void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}
