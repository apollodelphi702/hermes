#include <hermes/system.h>

DefineMailboxRequest(
    get_current_clock_rate,
    MBOX_TAG_GET_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST, 0 /* Placeholder (clock) */, 0
);

DefineMailboxRequest(
    get_max_clock_rate,
    MBOX_TAG_GET_MAX_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST, 0 /* Placeholder (clock) */, 0
);

unsigned int get_current_clock_speed(mbox_tag_clock_t clock) {
    mbox_set_tag_value_u32(mbox_msg_get_current_clock_rate, MBOX_TAG_GET_CLOCK_RATE, 0, clock);
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_current_clock_rate)) {
        return mbox_get_tag_value_u32(mbox_msg_get_current_clock_rate, MBOX_TAG_GET_CLOCK_RATE, 1);
    }

    return 0;
}

unsigned int get_max_clock_speed(mbox_tag_clock_t clock) {
    mbox_set_tag_value_u32(mbox_msg_get_max_clock_rate, MBOX_TAG_GET_MAX_CLOCK_RATE, 0, clock);
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_max_clock_rate)) {
        return mbox_get_tag_value_u32(mbox_msg_get_max_clock_rate, MBOX_TAG_GET_MAX_CLOCK_RATE, 1);
    }

    return 0;
}
