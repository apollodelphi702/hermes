#pragma once

#include <stdint.h>

/**
 * Performs a no-op that won't be optimized away.
 * This has the effect of waiting a CPU cycle.
 */
static inline void nop() {
    asm volatile("nop");
}

/**
 * Loop for n = count times.
 * This is done in such a way that the compiler won't optimize it out.
 * @param count The number of times to loop.
 */
static inline void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}
