#!/bin/sh
cwd=$(pwd)

if [ ! -d "./kernel" ]; then
  echo "You must be in the hermes project root directory to run this."
  exit 1
fi

# Switch to the CMake build directory (might be named differently).
if [ -d "./cmake-build-debug" ]; then cd ./cmake-build-debug || exit 2
elif [ -d "./build" ]; then cd ./build || exit 2
fi

# Build the system.
ninja hermes

# Switch back to the root.
cd "$cwd" || exit

# Run the emulator script.
./scripts/emulator.sh ./out/kernel.elf
