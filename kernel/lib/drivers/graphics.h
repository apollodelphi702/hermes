#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t virtual_width;
    uint32_t virtual_height;
    uint32_t depth;
} graphics_init_t;

/**
 * Initializes the graphics driver.
 * @param init The initialization structure, graphics_init_t.
 * @return A boolean, indicating success (true) or failure (false).
 */
bool graphics_init(graphics_init_t* init);

/**
 * Returns the framebuffer base address.
 * @return A pointer to the base of the graphics framebuffer.
 */
void* graphics_get_framebuffer();

/**
 * Gets the size of the framebuffer, in bytes.
 * @return The framebuffer size, in bytes, as an unsigned 32-bit integer.
 */
uint32_t graphics_get_framebuffer_size();
