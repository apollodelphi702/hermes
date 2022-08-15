#include <hermes/system.h>
#include "drivers/mbox.h"

DefineMailboxMessage(
    get_current_clock_rate,

    // Buffer
    8 * sizeof(uint32_t),
    MBOX_REQUEST,

    // Get Core Clock Rate (Tag 0)
    MBOX_TAG_GET_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_REQUEST, MBOX_TAG_CLOCK_ARM, 0,

    // End
    MBOX_TAG_END
);

DefineMailboxMessage(
    get_max_clock_rate,

    // Buffer
    8 * sizeof(uint32_t),
    MBOX_REQUEST,

    // Get Core Clock Rate (Tag 0)
    MBOX_TAG_GET_MAX_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_REQUEST, MBOX_TAG_CLOCK_ARM, 0,

    // End
    MBOX_TAG_END
);

unsigned int get_current_clock_speed() {
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_current_clock_rate)) {
        return mbox_msg_get_current_clock_rate[6];
    }

    return 0;
}

unsigned int get_max_clock_speed() {
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_max_clock_rate)) {
        return mbox_msg_get_max_clock_rate[6];
    }

    return 0;
}
