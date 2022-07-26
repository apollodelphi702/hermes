#!/bin/sh

# Which QEMU machine should be used to emulate the system.
Machine="raspi3b"

# Identify the kernel image to load.
KernelImage="$1"
if [ -z "$KernelImage" ]; then
  if [ -f ./out/kernel8.img ]; then
    KernelImage="./out/kernel8.img"
    echo "[INFO] No kernel image was specified, so the one in ./out was used."
  else
    echo "[ERROR] Failed to locate a kernel image."
    echo " -> Are you in the right directory and have you built the project?"
    exit 1
  fi
fi

echo "[INFO] Starting system with kernel image: $KernelImage"
echo " -> Machine: $Machine"
echo " -> Serial: stdio"
echo " -> GDB Monitor: localhost:4443 (TCP)"
echo " -> QMP (QEMU Debug): localhost:4444 (TCP)"
echo ""
echo "[INFO] Begin serial output from VM:"
echo "-----------------------------------"
qemu-system-aarch64 -M "$Machine" -kernel "$KernelImage" \
  -serial stdio \
  -monitor tcp:localhost:4443,server,nowait \
  -qmp tcp:localhost:4444,server,nowait
