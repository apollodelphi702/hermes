#include <drivers/graphics.h>
#include <drivers/mbox.h>

DefineMailboxRequest(
    initialize_framebuffer,
    MBOX_TAG_SET_PHYSICAL_SIZE, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST, 0 /* Width */, 0 /* Height */,
    MBOX_TAG_SET_VIRTUAL_SIZE,  2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST, 0 /* Width */, 0 /* Height */,
    MBOX_TAG_SET_DEPTH,         1 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST, 0 /* Depth */,
    0, 0, 0 /* Pad to 16-byte alignment */
);

DefineMailboxRequest(
    allocate_framebuffer,
    MBOX_TAG_ALLOCATE_BUFFER, 2 * sizeof(uint32_t), MBOX_TAG_STATE_REQUEST, 0 /* Placeholder (request 16-byte alignment) */, 0
);

static void* framebuffer;
static uint32_t framebuffer_size;

bool graphics_init(graphics_init_t* init) {
    mbox_init(mbox_msg_initialize_framebuffer);

    mbox_set_tag_value_u32(mbox_msg_initialize_framebuffer, MBOX_TAG_SET_PHYSICAL_SIZE, 0, init->width);
    mbox_set_tag_value_u32(mbox_msg_initialize_framebuffer, MBOX_TAG_SET_PHYSICAL_SIZE, 1, init->height);
    mbox_set_tag_value_u32(mbox_msg_initialize_framebuffer, MBOX_TAG_SET_VIRTUAL_SIZE, 0, init->virtual_width);
    mbox_set_tag_value_u32(mbox_msg_initialize_framebuffer, MBOX_TAG_SET_VIRTUAL_SIZE, 1, init->virtual_height);
    mbox_set_tag_value_u32(mbox_msg_initialize_framebuffer, MBOX_TAG_SET_DEPTH, 0, init->depth);

    // Attempt to initialize the framebuffer parameters.
    // If this fails, exit early.
    if (!mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_initialize_framebuffer)) return false;

    // Otherwise, we can try to allocate the framebuffer.
    mbox_init(mbox_msg_allocate_framebuffer);
    mbox_set_tag_value_u32(mbox_msg_allocate_framebuffer, MBOX_TAG_ALLOCATE_BUFFER, 0, 16);
    if (!mbox_call(MBOX_CHANNEL_ARM_TO_VC, mbox_msg_allocate_framebuffer)) return false;

    framebuffer         = (void*) (uint64_t) mbox_get_tag_value_u32(mbox_msg_allocate_framebuffer, MBOX_TAG_ALLOCATE_BUFFER, 0);
    framebuffer_size    = mbox_get_tag_value_u32(mbox_msg_allocate_framebuffer, MBOX_TAG_ALLOCATE_BUFFER, 1);
    return true;
}

void* graphics_get_framebuffer() {
    return framebuffer;
}

uint32_t graphics_get_framebuffer_size() {
    return framebuffer_size;
}
