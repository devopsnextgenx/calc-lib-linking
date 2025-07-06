#!/bin/bash

declare -A options
options=(
    ["test"]="TEST"
    ["release"]="BUILD_RELEASE"
    ["help"]="HELP"
)

BUILD_TYPE="Debug"
if [[ "$BUILD_RELEASE" == "ON" ]]; then
    BUILD_TYPE="Release"
fi

BUILD_DIR="build/$BUILD_TYPE"
OSX=$(uname -s)
CALCX="build/$BUILD_TYPE/calcx"
if [[ "$OSX" != "Linux" ]]; then
    CALCX="build/$BUILD_TYPE/Debug/calcx.exe"
    cp libraries/SDL3.dll build/$BUILD_TYPE/Debug/SDL3.dll
fi

"$CALCX" --version
echo "-----------------xxx-----------------------"
export SDL_VIDEODRIVER=x11
"$CALCX" --graphics