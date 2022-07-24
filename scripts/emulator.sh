#!/bin/sh

# Identify the kernel image to load.
KernelImage="$1"

echo "Starting system with kernel image: $KernelImage"
echo " -> RAM: 1GiB"
echo " -> QMP (QEMU Debug): localhost:4444"
qemu-system-aarch64 -M raspi3b -kernel "$KernelImage" -monitor stdio -qmp tcp:localhost:4444,server,nowait
