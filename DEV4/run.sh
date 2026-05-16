#!/bin/bash
# Linux build & launch script. Mirrors run.bat for Windows.
#
# Requirements:
#   - cmake >= 3.20
#   - ninja (sudo apt install ninja-build)
#   - libjack-dev (sudo apt install libjack-dev)
#   - libgl-dev  (sudo apt install libgl-dev)
#   - Initialised submodule: git submodule update --init libraries

set -e  # Stop on the first failing command.

ROOT="$(cd "$(dirname "$0")" && pwd)"

# Initialise the libraries submodule if the precompiled files are missing.
if [ ! -f "$ROOT/libraries/sdl/lib/linux-x86_64/libSDL3.a" ]; then
    echo "Initialising the libraries submodule..."
    git -C "$ROOT" submodule update --init libraries
fi

echo "Configuring with CMake..."
cmake -S "$ROOT" -B "$ROOT/cmake-build-debug" \
    -DCMAKE_BUILD_TYPE=Debug \
    -G Ninja

echo "Building..."
cmake --build "$ROOT/cmake-build-debug" --config Debug

echo "Launching..."
"$ROOT/cmake-build-debug/beatbox_64464"
