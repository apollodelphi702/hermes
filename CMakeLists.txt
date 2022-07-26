cmake_minimum_required(VERSION 3.23)
project(hermes)

enable_language(ASM)

set(CMAKE_C_STANDARD 23)

# Set build system locations.
get_filename_component(HERMES_SCRIPTS_DIRECTORY     "./scripts" ABSOLUTE)
get_filename_component(HERMES_OUTPUT_DIRECTORY      "./out" ABSOLUTE)

# Set up Assembly and C cross-compilers.
message("Configuring cross-compilers")
set(CMAKE_ASM_COMPILER aarch64-elf-gcc)
set(CMAKE_ASM_LINK_EXECUTABLE aarch64-elf-as)
set(CMAKE_C_COMPILER aarch64-elf-gcc)
set(CMAKE_C_LINK_EXECUTABLE_NAME aarch64-elf-gcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Locate programs used / likely to be used by the toolchain.
find_program(OBJCOPY NAMES "objcopy" "aarch64-elf-objcopy")
if (NOT OBJCOPY)
    message(FATAL_ERROR "Failed to find objcopy. If you're on Debian/Ubuntu, you might need to install binutils which"
            " can be done with `sudo apt install binutils-aarch64-linux-gnu`. \
            On macOS, this can be done with `brew install aarch64-elf-binutils`.")
endif()

# mkfs.fat and mtools are not currently used by the toolchain yet, though they will
# probably be useful regardless and they are likely to be used fairly soon.

find_program(MKFSFAT NAMES "mkfs.fat")
if (NOT MKFSFAT)
    message(FATAL_ERROR
            "You must install dosfstools. \
        On macOS, this can be done with `brew install dosfstools`. \
        On Debian/Ubuntu, this can be done with `sudo apt install dosfstools`."
            )
endif()

find_program(MTOOLS NAMES "mtools")
if (NOT MTOOLS)
    message(FATAL_ERROR
            "You must install mtools. \
        On macOS, this can be done with `brew install mtools`. \
        On Debian/Ubuntu, this can be done with `sudo apt install mtools`."
            )
endif()

# Add components
add_subdirectory(boot)
add_subdirectory(kernel)

# Add hermes build target to build entire project.
add_custom_target(hermes ALL)
add_dependencies(hermes kernel.elf)
