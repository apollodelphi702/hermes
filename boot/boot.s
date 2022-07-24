// Hermes Microkernel Bootloader

// Labels all following data as part of the .text.boot section
// (boot code). We can use this label in our linker script to
// ensure this is placed before our kernel and is thus executed
// first.
.section ".text.boot"

// Reserved addresses for spinlocking CPUs.
.org 0xd8
.globl spin_cpu
spin_cpu:
    .quad 0 // CPU 0 @ 0xd8
    .quad 0 // CPU 1 @ 0xe0
    .quad 0 // CPU 2 @ 0xe8

    // CPU 3 @ 0xf0
    // This value is shared with the firmware, which initially reads two words
    // containing the magic number and version, then clears them, allowing us
    // to use this as CPU 3's spinlock address.
    .word 0x5afe570b    // Firmware Magic Number
    .word 0             // Firmware Version


// Reserved address for 32-bit device tree blob pointer.
.org 0xf8
.globl device_tree_ptr
device_tree_ptr:
    .word 0x100

// Reserved address for 32-bit kernel entry pointer.
.org 0xfc
.globl kernel_entry_ptr
kernel_entry_ptr:
    .word 0x80000

// Label for the device tree.
.org 0x100
.globl device_tree

// Move to memory address 0x80000.
.org 0x80000

// Entry point for kernel.
// Execution is 'dropped' to us here by the Raspberry Pi firmware's
// armstub8.s once the Raspberry Pi has initialized itself.
.globl _start
_start:
    // Place all non-primary cores in spinlock.
    // This is more done for compatability reasons. Newer firmwares should automatically do this but older ones do not
    // and it seems neither does QEMU.

    // The registers at this point are set as follows:
    // x0 -> 32-bit pointer to the device tree blob on the primary core (or 0 on secondary cores).
    // The device tree blob on ARM devices (to put it very simply) replaces the use of the BIOS or ACPI in x86
    // systems to locate peripherals and other devices in the system.
    // x1 -> 0 (reserved for future use)
    // x2 -> 0 (reserved for future use)
    // x3 -> 0 (reserved for future use)
    // (x4 -> 32-bit kernel entry point, _start location)

    // Move the value of the current coprocessor's Multiprocessor Affinity Register register into x6.
    mrs x6, MPIDR_EL1
    // Enable only the first CPU by ANDing the coprocessor register value with 3.
    // This causes only the first two bits to be checked, which per the ARM documentation is the highest level
    // affinity value of the current core - each core has a unique value and there are 4 possible values of (0 to 3).
    // Though the ARM spec. defines for up to 8 cores, the Raspberry Pi only uses 4 and values written by the firmware
    // conflict with the CPU spinlock table.
    and x6, x6, #0b011
    // Checking the first three bits of this means that we'll only get a zero when the current core is the first in
    // the system (i.e., when the CPUID is 0.) (Additionally this is 0 for the only core in a uniprocessor system,
    // ensuring that case is covered too).
    // Thus, if this is equal to 0, we'll jump over the spinlock (to local label 2) to initialize the kernel.
    cbz x6, 2f

    // Otherwise we'll set the current processor to spinlock by setting it to a spinlock address.
    // If we want to wake this core later we can assign the address of the code we want that core to start executing to
    // its spinlock address and interrupt its spinlock.
    adr x5, spin_cpu

    // Enter power-efficient spinlock using wfe.
1:  wfe
    // Load into x4, the value of the memory address found by:
    //      taking the value of x5, offset by x6 (where x6 is first shifted left by 3).
    // - Shifting left by 3 has the same effect as multiplying by 8.
    // - x5 is the base address of the CPU spinlock table.
    // - x6 will be the CPUID (current processor number).
    // (x5 and x6 are set above and won't change on the spinlocking cores.)
    // Thus, the value we store in x4 is the value of the entry for the current CPU in the spinlock table.
    // (Every 8 bytes - or .quad) in the spinlock table is a CPU's spinlock address.
    ldr x4, [x5, x6, lsl #3]
    // If the value contained in x4 is zero meaning an address hasn't been assigned to this CPU to start executing via
    // the spinlock table, continue spinlocking.
    cbz x4, 1b
    // Otherwise we'll continue onward and boot up this processor.

    // Clear the parameter registers.
    // x0 is ordinarily the device tree blob address, but on secondary processors it should be set to 0 so we do that
    // here. x1, x2 and x3 should be set to 0 anyway.
    mov x0, #0
    mov x1, #0
    mov x2, #0
    mov x3, #0
    // Then we branch to the address stored in register x4 (which would be the value that was set in the spinlock
    // table for this core).
    // We don't need to set/change/move x4 again because its intended value is the kernel entry point.
    br x4

    // Load the address of _start into x5 as an intermediate value (necessary on ARM due to the lack of
    // specialized memory manipulation instructions) and then move that value into the stack pointer.
    // This ensures the stack starts at the beginning of our code.
2:  ldr x5, =_start
    mov sp, x5

    // Zero-initialize the '.bss' section (very important!)
    // The .bss section (colloquially known as the 'better save space' section) is where all your zero-initialized
    // values live. Forgetting to actually zero-initialize this can cause lots of weird bugs - specifically of the kind
    // that weirdly manifests itself when you move from an emulator to real hardware.
    ldr x5, =__bss_start
    ldr w6, =__bss_size

    // CBZ = compare and branch on zero.
    // CBNZ = compare and branch on not zero.
    // These instructions are useful to avoid changing the ARM condition flags and to reduce the number of
    // instructions necessary to perform comparisons.

    // Here if w6 is zero - meaning we've cleared all of BSS, we do a forward jump to the local label 4 to boot the
    // kernel by calling into kernel_main.
    // Local Labels are a GNU-assembler specific feature, that allow 'temporarily' defining a label for the assembler
    // through the use of a numeric index - the first 10 of which (0 to 9) are implemented in a slightly more
    // efficient manner than the others.
3:  cbz w6, 4f
    // The zero register is the register equivalent of /dev/null - it is filled with zeroes.
    // This stores the value of xzr (zeroes) in the memory address of x5 and then adds 8 to the address stored in x5.
    // Essentially blanking out the byte of memory that x5 points to and then shifting x5 to the next byte.
    str xzr, [x5], #8
    // One is subtracted from w6 which, above, was initialized with the size of the .bss section.
    sub w6, w6, #1
    // If w6 is not yet zero, jumps backwards to local label 3 to clear the next byte of memory and continue.
    // Otherwise, this instruction is ignored and we move onto the next line of assembly.
    cbnz w6, 3b

    // Now we're ready to jump to our kernel main function.
    // This shouldn't return, but as a failsafe we'll add a halt instruction right after.
    //
    // Per the ARM calling convention, the first three registers will be passed as arguments to the function.
    // (x0, x1, x2 and x3).
4:  mov x1, #0
    mov x2, #0
    mov x3, #0

    // Load the device tree pointer and kernel entry points into x0 and x4 respectively.
    // Though the values themselves are 32-bit, they're loaded into 64-bit registers to clear the upper bits.
    // (As the convention in the kernel_main, for example, is to use a uint64_t for the device tree pointer.)
    ldr x0, device_tree_ptr
    ldr x4, kernel_entry_ptr

    // Branch to kernel_main.
    // We should never return from kernel_main.
    // We could branch link (bl) to kernel_main instead to ensure that the system halts afterwards (it might also be
    // necessary to unalign or just branch to halt afterwards), however not using branch link avoids placing the return
    // address on the stack.
    b kernel_main

// Jumping to this address allows for halting the system.
.align 16
.globl halt
halt:
    // Enter power-efficient spinlock using wfe.
    // We do this in a loop because if the CPU is awoken we still have nothing for it to do so we'll re-enter
    // the spinlock.
    wfe
    b halt
