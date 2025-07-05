# Cross-Platform Build Instructions

This project has been configured to build on both Linux/Unix and Windows systems.

## Platform Requirements

### Linux/Unix
- CMake 3.30 or higher
- GCC or Clang compiler with C++17 support
- SDL3 library (`.so` or `.so.0` file)
- Math library (automatically linked)

### Windows
- CMake 3.30 or higher
- Visual Studio 2017 or higher (MSVC), or MinGW-w64
- SDL3 library (`.lib` and `.dll` files)

### macOS
- CMake 3.30 or higher
- Xcode or command line tools
- SDL3 library (`.dylib` file)

## Library Structure

Place your SDL3 libraries in the `libraries/` directory:

```
libraries/
├── libSDL3.so.0        # Linux shared library
├── libSDL3.dylib       # macOS dynamic library
├── SDL3.lib            # Windows import library
├── SDL3main.lib        # Windows main library (optional)
└── SDL3.dll            # Windows dynamic library
```

## Building

### Linux/Unix (using build.sh)
```bash
# Make the script executable
chmod +x build.sh

# Build everything with shared libraries
./build.sh exe calc graphics shared test

# Build with static libraries (default)
./build.sh exe calc graphics static test

# Build in release mode
./build.sh exe calc graphics release test

# Show help
./build.sh help
```

### Windows (using build.bat)
```cmd
REM Build everything with shared libraries
build.bat exe calc graphics shared test

REM Build with static libraries (default)
build.bat exe calc graphics static test

REM Build in release mode
build.bat exe calc graphics release test

REM Show help
build.bat help
```

### Manual CMake (All Platforms)
```bash
# Create build directory
mkdir build
cd build

# Configure (Linux/macOS)
cmake .. -DBUILD_CALC_LIB=ON -DBUILD_GRAPHICS_LIB=ON -DBUILD_CALCX_EXE=ON -DBUILD_SHARED=OFF

# Configure (Windows with Visual Studio)
cmake .. -DBUILD_CALC_LIB=ON -DBUILD_GRAPHICS_LIB=ON -DBUILD_CALCX_EXE=ON -DBUILD_SHARED=OFF -G "Visual Studio 17 2022"

# Build
cmake --build . --config Debug
```

## Build Options

- `BUILD_CALC_LIB`: Build the calc library (ON/OFF)
- `BUILD_GRAPHICS_LIB`: Build the graphics library (ON/OFF)
- `BUILD_CALCX_EXE`: Build the calcx executable (ON/OFF)
- `BUILD_SHARED`: Build shared libraries instead of static (ON/OFF)

## Platform-Specific Notes

### Windows
- The build system automatically detects and links against SDL3 libraries
- Static linking uses MultiThreaded runtime when available
- Supports both Visual Studio and MinGW generators

### Linux
- Automatically links the math library (`-lm`)
- Supports static linking with `-static` flag
- Looks for SDL3 in both local libraries/ directory and system paths

### macOS
- Supports both local and system SDL3 installations
- Automatically handles framework linking if needed

## Troubleshooting

1. **SDL3 not found**: Ensure SDL3 libraries are in the `libraries/` directory with correct naming
2. **Build failures on Windows**: Try using Visual Studio developer command prompt
3. **Static linking issues**: Some platforms may not support full static linking with SDL3
4. **Missing math library errors**: This is automatically handled per platform

## Generated Files

After building, you'll find:
- Libraries in `build/Debug/` or `build/Release/`
- Executables in the same directory
- Platform-specific extensions (`.exe` on Windows, no extension on Unix)
