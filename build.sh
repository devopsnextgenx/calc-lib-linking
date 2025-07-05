#!/bin/bash

declare -A options
options=(
    ["exe"]="BUILD_EXE"
    ["calc"]="BUILD_CALC_LIB"
    ["graphics"]="BUILD_GRAPHICS_LIB"
    ["static"]="BUILD_STATIC"
    ["test"]="TEST"
    ["release"]="BUILD_RELEASE"
    ["help"]="HELP"
)



BUILD_CALC_LIB=OFF
BUILD_GRAPHICS_LIB=OFF
BUILD_LIB=OFF
BUILD_EXE=OFF
BUILD_SHARED=OFF
TEST=OFF

# loop through the arguments and set the corresponding variables
for arg in "$@"; do
    if [[ -n "${options[$arg]}" ]]; then
        declare "${options[$arg]}"=ON
    fi
done

if [[ "$HELP" == "ON" ]]; then
    echo "Usage: $0 [exe] [calc] [graphics] [static] [test] [release] [help]"
    echo "Options:"
    echo "  exe        Build the executable"
    echo "  calc       Build the calc library"
    echo "  graphics   Build the graphics library"
    echo "  static     Build static libraries, if not specified, shared libraries will be built"
    echo "  test       Run tests after building"
    echo "  release    Build in release mode, if not specified, debug mode will be used"
    echo "  help       Show this help message"
    exit 0
fi

if [ "$BUILD_STATIC" == "ON" ]; then
    BUILD_SHARED=OFF
else
    BUILD_SHARED=ON
fi

BUILD_TYPE="Debug"
if [[ "$BUILD_RELEASE" == "ON" ]]; then
    BUILD_TYPE="Release"
fi
BUILD_DIR="build/$BUILD_TYPE"
mkdir -p "$BUILD_DIR"

# Clean up CMake cache in the build dir
rm -f "$BUILD_DIR/CMakeCache.txt"
rm -rf "$BUILD_DIR/CMakeFiles"

cd "$BUILD_DIR"

# Use the absolute path to the project root as the source directory
echo "cmake ../.. -DBUILD_CALC_LIB=$BUILD_CALC_LIB -DBUILD_GRAPHICS_LIB=$BUILD_GRAPHICS_LIB -DBUILD_CALCX_EXE=$BUILD_EXE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_SHARED=$BUILD_SHARED"
cmake ../.. -DBUILD_CALC_LIB=$BUILD_CALC_LIB -DBUILD_GRAPHICS_LIB=$BUILD_GRAPHICS_LIB -DBUILD_CALCX_EXE=$BUILD_EXE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_SHARED=$BUILD_SHARED
echo "-------------------------------------------"
cmake --build .
echo "-------------------------------------------"

cd ../..
#  for windows use the absolute path to the executable ends with .exe
OSX=$(uname -s)
CALCX="build/$BUILD_TYPE/calcx"
if [[ "$OSX" != "Linux" ]]; then
    CALCX="build/$BUILD_TYPE/Debug/calcx.exe"
    cp libraries/SDL3.dll build/$BUILD_TYPE/Debug/SDL3.dll
fi

if [[ -f "$CALCX" && "$BUILD_EXE" == "ON" && "$TEST" == "ON" ]]; then
    echo "Running $CALCX"
    if [[ "$BUILD_GRAPHICS_LIB" == "ON" ]]; then
        echo "-------------------------------------------"
        export SDL_VIDEODRIVER=x11
        "$CALCX" --graphics
        echo "-----------------xxx-----------------------"
        unset SDL_VIDEODRIVER
    fi
    if [[ "$BUILD_CALC_LIB" == "ON" ]]; then
        echo "-------------------------------------------"
        "$CALCX" --calc
        echo "-----------------xxx-----------------------"
    fi
    echo "-------------------------------------------"
    "$CALCX" --version
    echo "-----------------xxx-----------------------"
    ls -ltr "$CALCX"
    echo "-------------------------------------------"
    echo "Executable $CALCX executed successfully."
    echo "Build and execution completed successfully."
else
    if [[ "$BUILD_EXE" == "ON" && "$TEST" == "ON" ]]; then
        echo "Build failed or executable not found."
    else
        echo "Executable $CALCX not built. Skipping execution."
    fi
fi