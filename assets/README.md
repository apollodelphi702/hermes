# Hermes System Assets
These assets are copied to the system image at build time.

## `/root`
Anything in [`root/`](./root) will be copied to the root directory of the boot medium.

- `start.elf`; the firmware (equivalent to the BIOS on an IBM PC).
- `bootcode.bin`; loaded first and executed on the GPU.
- `fixup.dat`; important hardware-related information.

The above files are obtained from the [official firmware repository](https://github.com/raspberrypi/firmware/tree/master/boot).
