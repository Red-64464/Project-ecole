#!/bin/bash
# Script de build et lancement pour Linux.
# Equivalent de run.bat mais pour Linux.
#
# Prerequis :
#   - cmake >= 3.20
#   - ninja (sudo apt install ninja-build)
#   - libjack-dev (sudo apt install libjack-dev)
#   - libgl-dev  (sudo apt install libgl-dev)
#   - git submodule initialise : git submodule update --init libraries

set -e  # Arrete le script si une commande echoue

ROOT="$(cd "$(dirname "$0")" && pwd)"

# Initialise le sous-module des bibliotheques si pas encore fait
if [ ! -f "$ROOT/libraries/sdl/lib/linux-x86_64/libSDL3.a" ]; then
    echo "Initialisation du sous-module libraries..."
    git -C "$ROOT" submodule update --init libraries
fi

echo "Configuration CMake..."
cmake -S "$ROOT" -B "$ROOT/cmake-build-debug" \
    -DCMAKE_BUILD_TYPE=Debug \
    -G Ninja

echo "Compilation..."
cmake --build "$ROOT/cmake-build-debug" --config Debug

echo "Lancement..."
"$ROOT/cmake-build-debug/beatbox_64464"
