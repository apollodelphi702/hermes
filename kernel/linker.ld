/*
    Hermes Linker Script
    --
    When an executable is built, all the component object files are joined (or 'linked') together by a program
    called a linker. The linker script can customize the behavior of the linker in terms of where it places
    sections of the object file (memory layout).

    Additionally, extra instructions can be given to the linker - particularly useful here as we have certain
    requirements with respect to how our final object file should be organized.
*/

/* -------- */

/*
	Set the kernel entry point to the _start method.
	This method accepts the handoff arguments from the bootloader and initializes the kernel.
*/
ENTRY(_start)

SECTIONS {

    /* Set the starting address to LOADER_ADDR. */
    /* For AArch64, this is 0x80000. */
    . = 0x80000;

    __start = .;
    __text_start = .;

    /* .text - usually program code. This is often read-only but executable. */
    .text : {
        /* Ensure .text.boot is not stripped out (and by ordering it first, ensure it is included first) */
        KEEP(*(.text.boot))
        /* Add remaining .text sections. */
        *(.text .text.*)
    }

    . = ALIGN(4096); /* Ensure the end of the section is page-aligned. */

    __text_end = .;
    __rodata_start = .;

    /* .rodata - Read-Only data (not executable, but not writable either). */
    .rodata : {
        *(.rodata .rodata.*)
    }

    . = ALIGN(4096); /* Ensure the end of the section is page-aligned. */

    __rodata_end = .;
    __data_start = .;

    /* .data - readable-writeable but not executable data. */
    .data : {
        *(.data .data.*)
    }

    . = ALIGN(4096); /* Ensure the end of the section is page-aligned. */

    __data_end = .;
    __bss_start = .;

    /* .bss (block starting symbol // 'better save space') - uninitialized variables */
    .bss : {
        /* Set the bss label to the current address and then include any uninitialized variables. */
        bss = .;
        *(.bss .bss.*)
        *(COMMON)
    }

    . = ALIGN(4096); /* Ensure the end of the section is page-aligned. */

    __bss_end = .;
    /* Quick Math - we can do arithmetic with the labels to define a useful __bss_size symbol. */
    /* This symbol is used in the bootloader to clear out the .bss section. */
    __bss_size = __bss_end - __bss_start;
    __end = .;

}
