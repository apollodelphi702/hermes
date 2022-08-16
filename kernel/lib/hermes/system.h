#pragma once

#include <drivers/mbox.h>

/**
 * Gets the current clock speed (Hz) of the specified clock.
 * @param clock The clock to get the speed of.
 * @return The speed in Hz as an unsigned 32-bit integer.
 */
uint32_t get_current_clock_speed(mbox_tag_clock_t clock);

/**
 * Gets the maximum possible clock speed (Hz) of the specified clock.
 * @param clock The clock to get the maximum speed of.
 * @return The maximum speed in Hz as an unsigned 32-bit integer.
 */
uint32_t get_max_clock_speed(mbox_tag_clock_t clock);

/**
 * Gets the minimum possible clock speed (Hz) of the specified clock.
 * @param clock The clock to get the minimum speed of.
 * @return The minimum speed in Hz as an unsigned 32-bit integer.
 */
uint32_t get_min_clock_speed(mbox_tag_clock_t clock);

/**
 * Set the current clock speed (Hz) of the specified clock. Skipping the turbo setting if skip_turbo is true.
 * @param clock The clock to set the speed of.
 * @param speed The speed in Hz to set the clock to.
 * @param skip_turbo Whether to skip the turbo setting. (True = yes, any other value = no).
 * @return Whether setting the clock speed was successful.
 */
bool set_current_clock_speed(mbox_tag_clock_t clock, uint32_t speed, bool skip_turbo);
