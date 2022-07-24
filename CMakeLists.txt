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

# Add components
add_subdirectory(boot)
add_subdirectory(kernel)

# Add hermes build target to build entire project.
add_custom_target(hermes ALL)
add_dependencies(hermes kernel.elf)
