#!/bin/sh
cwd=$(pwd)

# Whether the emulator should be started after building.
RUN_EMULATOR=1

if [ ! -d "./kernel" ] || [ ! -f "./hermes" ]; then
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

while test $# -gt 0; do
  if test "$1" = "--skip-emulator"; then RUN_EMULATOR=0; shift; continue; fi
done

if [ $RUN_EMULATOR -gt 0 ]; then
  # Run the emulator script.
  ./scripts/emulator.sh ./out/kernel8.img
fi
