#pragma once

#include <drivers/mbox.h>

unsigned int get_current_clock_speed(mbox_tag_clock_t clock);

unsigned int get_max_clock_speed(mbox_tag_clock_t clock);
