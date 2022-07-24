# For now, just include any C files recursively as kernel files.
# This might need to be revisited in future.
file(GLOB_RECURSE HERMES_KERNEL_SOURCES *.c)

# Set compiler and linker flags.
# -O2; set compiler optimization to level 2 for speed.
#   (-O1 is often best, but -O2 offers better performance where it works properly).
#   (-O3 is max but basically destroys your code).
#   (There is also -Os to optimize for size.)
#   It's usually worth some experimentation with these.
# -Wall -Wextra -Wpedantic; enable all compiler warnings (plus additional extra ones).
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra -Wpedantic")
set(CMAKE_C_LINK_FLAGS "-ffreestanding -O2 -nostdlib -lgcc -T kernel/linker.ld")
set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_LINK_EXECUTABLE_NAME} <CMAKE_C_LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
configure_file(linker.ld linker.ld COPYONLY)

# Add kernel executable target with sources.
add_executable(kernel.elf "${HERMES_KERNEL_SOURCES}")

# Link kernel with additional objects or dependencies.
#...

# Link kernel with bootloader stub.
target_link_libraries(kernel.elf boot.o)

# Add kernel library
set_property(
        TARGET kernel.elf
        APPEND PROPERTY
            INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# Copy kernel image to output directory.
add_custom_command(TARGET kernel.elf POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy kernel.elf ${HERMES_OUTPUT_DIRECTORY}/kernel.elf)

## Pack kernel executable into kernel8.img file.
#add_custom_command(TARGET kernel.elf POST_BUILD
#        COMMAND aarch64-elf-objcopy kernel.elf -O binary kernel8.img)
#
## Copy kernel8.img to output directory.
#add_custom_command(TARGET kernel.elf POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy kernel8.img ${HERMES_OUTPUT_DIRECTORY}/kernel8.img)
