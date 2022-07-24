# Include any assembly files as bootloader files.
# ...though there will probably only be one.
file(GLOB_RECURSE HERMES_BOOT_SOURCES *.s)

# Use aarch64-elf-as to build an intermediate object file for the bootloader.
# We don't want to build a straight up executable, yet, because this is just a stub that will be linked into our kernel
# binary to make it bootable.
#add_custom_target(boot
#        COMMAND aarch64-elf-as -c ${HERMES_BOOT_SOURCES} -o boot.o
#        COMMENT "Assembling bootloader"
#        BYPRODUCTS boot.o)
add_library(boot.o OBJECT boot.s)
