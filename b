#!/bin/bash

set -e
cd "$(dirname "$0")"
mkdir -p build
cd build

# if [ ! -f CMakeCache.txt ] && [ "$1" != "clean" ]; then
#     cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=OFF
# fi

if [ ! -f CMakeCache.txt ] && [ "$1" != "clean" ]; then
    echo "No existing build configuration found. Initializing Debug (${BACKEND_CHOICE})..."
    cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=OFF -DBACKEND=${BACKEND_CHOICE}
fi

BACKEND_CHOICE="SDL2"
if [ "${2,,}" = "raylib" ]; then
    BACKEND_CHOICE="RAYLIB"
fi

echo "Using: ${BACKEND_CHOICE}"

case "$1" in
    d|debug)
        cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=OFF -DBACKEND=${BACKEND_CHOICE}
        cmake --build .
        ;;
    f|frelease)
        cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_ASAN=OFF -DBACKEND=${BACKEND_CHOICE}
        cmake --build .
        ;;
    a|asan|sanitize)
        cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON -DBACKEND=${BACKEND_CHOICE}
        cmake --build .
        ;;
    r|run)
        # cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=OFF -DBACKEND=${BACKEND_CHOICE}
        cmake --build . --target run
        ;;
    val|valgrind)
        cmake --build . --target valgrind
        ;;
    clean)
        echo "Cleaning build directory..."
        find . -mindepth 1 -delete
        ;;
    *)
      echo "Usage: ./b.sh [(d)ebug| (f)inalrelease|(a)san|(r)un|(val)grind|clean] [ -> sdl2 <- |raylib]"
        echo "  d, debug     - Debug build (symbols active, ASan off)"
        echo "  r, release   - Release build (optimized, ASan off)"
        echo "  a, asan      - Debug build with AddressSanitizer enabled"
        echo "  run          - Run the program"
        echo "  val, valgrind - Run with valgrind memory analysis"
        echo "  clean        - Safely wipe build directory"
        ;;
esac
