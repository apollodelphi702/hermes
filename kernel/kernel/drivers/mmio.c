#include <drivers/mmio.h>

/**
 * The current board type for the MMIO driver.
 */
static unsigned int MMIO_BOARD_TYPE;

/**
 * The base address for memory mapped IO.
 * This is set when the MMIO driver is initialized with mmio_init.
 */
static uint32_t MMIO_BASE = 0;

bool is_mmio_initialized() {
    return MMIO_BASE != 0;
}

unsigned int get_mmio_board_type() {
    return MMIO_BOARD_TYPE;
}

void mmio_init() {
    // Ensure MMIO hasn't already been initialized.
    if (is_mmio_initialized()) return;

    // Look up the board (by reading MIDR_ELx system register)
    uint32_t midr_el1;
    __asm__ volatile("mrs %x0, midr_el1" : "=r" (midr_el1));

    // The model is encoded in the PartNum value (bits [15:4]).
    // Interpret the PartNum value to retrieve the board number.
    int boardType = 0;
    switch ((midr_el1 >> 4) & 0xFFF) {
        case 0xD03: boardType = 3; break;
        case 0xD08: boardType = 4; break;
    }

    // Save the board type for later use.
    MMIO_BOARD_TYPE = boardType;

    // Set the MMIO base address depending on the board type/version.
    switch (boardType) {
        case 2:         /* Unsupported */ break;
        case 3:
            MMIO_BASE = 0x3F000000;
            break;
        case 4:
            MMIO_BASE = 0xFE000000;
            break;
        default:        /* Unsupported */
//            MMIO_BASE = 0x20000000;
            break;
    }
}

uint32_t mmio_read(uint64_t offset) {
    return *(volatile uint32_t*)(MMIO_BASE + offset);
}

void mmio_write(uint64_t offset, uint32_t value) {
    *(volatile uint32_t*) (MMIO_BASE + offset) = value;
}
