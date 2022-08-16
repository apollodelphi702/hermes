# The Hermes Microkernel
A Raspberry Pi 3+ Microkernel Operating System.

## Getting Started
To get started, ensure you've installed the necessary prerequisite software and set up your
toolchain. You can see the project wiki for more information about this.

Once you've done that, you can get a copy of the system built and running in QEMU by running
the following in the project root directory:
```bash
$ ./hermes run
```

## Running on real hardware
1. Start with an [official Raspberry Pi SD Card image](https://www.raspberrypi.com/documentation/computers/getting-started.html#installing-the-operating-system).
   - This is not required but makes sourcing [the necessary firmware files](https://github.com/raspberrypi/firmware/tree/master/boot) easier.
2. Run a build of Hermes to generate a working version.
    ```bash
    $ ./hermes clean
    $ ./hermes build
    ```
3. Copy the built kernel image (`kernel8.img`) to the SD card.
   ```bash
   $ cp ./out/kernel8.img /Volumes/boot/kernel8.img
   ```
4. Unmount the disk, eject the SD card and run it on a real Pi.
