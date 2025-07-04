#!/bin/bash

BUILD_LIB=OFF
BUILD_EXE=OFF

if [ "$1" == "exe" ]; then
    BUILD_EXE=ON
elif [ "$1" == "lib" ]; then
    BUILD_LIB=ON
fi

if [ "$2" == "exe" ]; then
    BUILD_EXE=ON
elif [ "$2" == "lib" ]; then
    BUILD_LIB=ON
fi

BUILD_CALC_SHARED=OFF

if [ "$3" == "shared" ]; then
    BUILD_CALC_SHARED=ON
fi

BUILD_TYPE="Release"
BUILD_DIR="build/$BUILD_TYPE"
mkdir -p "$BUILD_DIR"

# Clean up CMake cache in the build dir
rm -f "$BUILD_DIR/CMakeCache.txt"
rm -rf "$BUILD_DIR/CMakeFiles"

cd "$BUILD_DIR"

# Use the absolute path to the project root as the source directory
cmake ../.. -DBUILD_CALC_LIB=$BUILD_LIB -DBUILD_CALCX_EXE=$BUILD_EXE -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_CALC_SHARED=$BUILD_CALC_SHARED
echo "-------------------------------------------"
cmake --build .
echo "-------------------------------------------"

cd ../..
CALCX="build/$BUILD_TYPE/calcx"
if [ -f "$CALCX" ]; then
    echo "Running $CALCX"
    "$CALCX"
    echo "-------------------------------------------"
    ls -ltr "$CALCX"
    echo "-------------------------------------------"
    echo "Executable $CALCX executed successfully."
    echo "Build and execution completed successfully."
else
    echo "Executable $CALCX not found. Build may have failed."
fi