#include <hermes/system.h>

DefineMailboxRequest(
    get_current_clock_rate,
    MBOX_TAG_GET_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST,
    0 /* Placeholder (clock) */, 0
);

DefineMailboxRequest(
    get_max_clock_rate,
    MBOX_TAG_GET_MAX_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST,
    0 /* Placeholder (clock) */, 0
);

DefineMailboxRequest(
    get_min_clock_rate,
    MBOX_TAG_GET_MIN_CLOCK_RATE, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST,
    0 /* Placeholder (clock) */, 0
);

DefineMailboxRequest(
    set_clock_rate,
    MBOX_TAG_SET_CLOCK_RATE, 3 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST,
    0 /* Placeholder (clock) */, 0 /* Placeholder (rate) */, 0 /* Placeholder (skip turbo setting?) */
);

uint32_t get_current_clock_speed(mbox_tag_clock_t clock) {
    mbox_init(mbox_msg_get_current_clock_rate);

    mbox_set_tag_value_u32(mbox_msg_get_current_clock_rate, MBOX_TAG_GET_CLOCK_RATE, 0, clock);
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_current_clock_rate)) {
        return mbox_get_tag_value_u32(mbox_msg_get_current_clock_rate, MBOX_TAG_GET_CLOCK_RATE, 1);
    }

    return 0;
}

uint32_t get_max_clock_speed(mbox_tag_clock_t clock) {
    mbox_init(mbox_msg_get_max_clock_rate);

    mbox_set_tag_value_u32(mbox_msg_get_max_clock_rate, MBOX_TAG_GET_MAX_CLOCK_RATE, 0, clock);
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_max_clock_rate)) {
        return mbox_get_tag_value_u32(mbox_msg_get_max_clock_rate, MBOX_TAG_GET_MAX_CLOCK_RATE, 1);
    }

    return 0;
}

uint32_t get_min_clock_speed(mbox_tag_clock_t clock) {
    mbox_init(mbox_msg_get_min_clock_rate);

    mbox_set_tag_value_u32(mbox_msg_get_min_clock_rate, MBOX_TAG_GET_MIN_CLOCK_RATE, 0, clock);
    if (mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_get_min_clock_rate)) {
        return mbox_get_tag_value_u32(mbox_msg_get_min_clock_rate, MBOX_TAG_GET_MIN_CLOCK_RATE, 1);
    }

    return 0;
}

bool set_current_clock_speed(mbox_tag_clock_t clock, uint32_t speed, bool skip_turbo) {
    mbox_init(mbox_msg_set_clock_rate);

    mbox_tag_value_t clockSpeed = {
        .tag = MBOX_TAG_SET_CLOCK_RATE,
        .byte_length = 3 * sizeof(uint32_t),
        .data.buffer_u32 = {clock, speed, skip_turbo}
    };
    mbox_set_tag_value(mbox_msg_set_clock_rate, 0, &clockSpeed);
    return mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_set_clock_rate);
}
