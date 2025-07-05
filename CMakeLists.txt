cmake_minimum_required(VERSION 3.30)
project(calc-lib-linking C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection and configuration
if(WIN32)
    message(STATUS "Building for Windows")
    # Enable Windows-specific features if needed
    add_definitions(-DWIN32_LEAN_AND_MEAN)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Building for Linux")
elseif(APPLE)
    message(STATUS "Building for macOS")
endif()

# Set source and header directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libraries)

set(CALC_LIB ${SRC_DIR}/calc)
set(GRAPHICS_LIB ${SRC_DIR}/graphics)
set(CALCX_EXE ${SRC_DIR}/calcx)

# Options to control build
option(BUILD_CALC_LIB "Build calc shared library" ON)
option(BUILD_GRAPHICS_LIB "Build graphics shared library" ON)
option(BUILD_CALCX_EXE "Build calcx executable" ON)

# Option to control static/shared build
option(BUILD_SHARED "Build as a shared library" OFF)

# Platform-specific configurations
if(WIN32)
    set(SDL3_LIB_PATH "${CMAKE_SOURCE_DIR}/libraries")
    set(MATH_LIB "")  # Windows doesn't need explicit math library linking
    
    # On Windows, we need import libraries (.lib files) for linking
    # Look for SDL3 import library
    find_library(SDL3_LIBRARY 
        NAMES SDL3 SDL3.lib libSDL3 libSDL3.lib
        PATHS ${SDL3_LIB_PATH}
        PATH_SUFFIXES lib lib64
        NO_DEFAULT_PATH
    )
    
    find_library(SDL3_MAIN_LIBRARY 
        NAMES SDL3main SDL3main.lib libSDL3main libSDL3main.lib
        PATHS ${SDL3_LIB_PATH}
        PATH_SUFFIXES lib lib64
        NO_DEFAULT_PATH
    )
    
    # If .lib files are not found, try to link directly to the DLL (not recommended but may work)
    if(NOT SDL3_LIBRARY)
        if(EXISTS "${CMAKE_SOURCE_DIR}/libraries/SDL3.dll")
            message(WARNING "SDL3.lib import library not found. Attempting to link directly to SDL3.dll (not recommended)")
            set(SDL3_LIBRARY "${CMAKE_SOURCE_DIR}/libraries/SDL3.dll")
        endif()
    endif()
    
elseif(UNIX AND NOT APPLE)
    # Linux configuration
    if(EXISTS "${CMAKE_SOURCE_DIR}/libraries/libSDL3.so.0")
        set(SDL3_LIB "${CMAKE_SOURCE_DIR}/libraries/libSDL3.so.0")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/libraries/libSDL3.so")
        set(SDL3_LIB "${CMAKE_SOURCE_DIR}/libraries/libSDL3.so")
    else()
        # Try to find system SDL3
        find_library(SDL3_LIB NAMES SDL3 libSDL3)
    endif()
    set(MATH_LIB "m")  # Linux needs explicit math library linking
    
elseif(APPLE)
    # macOS configuration
    if(EXISTS "${CMAKE_SOURCE_DIR}/libraries/libSDL3.dylib")
        set(SDL3_LIB "${CMAKE_SOURCE_DIR}/libraries/libSDL3.dylib")
    else()
        # Try to find system SDL3
        find_library(SDL3_LIB NAMES SDL3)
    endif()
    set(MATH_LIB "m")  # macOS needs explicit math library linking
endif()

# Verify SDL3 was found
if(WIN32)
    if(NOT SDL3_LIBRARY)
        message(FATAL_ERROR "SDL3 library not found in ${SDL3_LIB_PATH}. 
        On Windows, you need SDL3.lib (import library) and SDL3.dll files.
        Please download the SDL3 development libraries for Windows from https://github.com/libsdl-org/SDL/releases
        and place SDL3.lib and SDL3main.lib in the libraries/ directory.")
    else()
        message(STATUS "Found SDL3 library: ${SDL3_LIBRARY}")
        if(SDL3_MAIN_LIBRARY)
            message(STATUS "Found SDL3main library: ${SDL3_MAIN_LIBRARY}")
        else()
            message(WARNING "SDL3main library not found. This may cause linking issues with main() function.")
        endif()
    endif()
else()
    if(NOT SDL3_LIB)
        message(WARNING "SDL3 library not found")
    else()
        message(STATUS "Found SDL3 library: ${SDL3_LIB}")
    endif()
endif()

if(BUILD_CALC_LIB)
    file(GLOB CALC_SOURCES "${CALC_LIB}/*.cpp")
    if(BUILD_SHARED)
        add_library(calc SHARED ${CALC_SOURCES})
        # On Windows, shared libraries need export macros for proper linking
        if(WIN32)
            target_compile_definitions(calc PRIVATE CALC_BUILDING_DLL)
            target_compile_definitions(calc INTERFACE CALC_USING_DLL)
        endif()
    else()
        add_library(calc STATIC ${CALC_SOURCES})
    endif()
    target_include_directories(calc PUBLIC ${INCLUDE_DIR})
    set_target_properties(calc PROPERTIES OUTPUT_NAME "calc")
    if(MATH_LIB)
        target_link_libraries(calc PUBLIC ${MATH_LIB})
    endif()
endif()

if(BUILD_GRAPHICS_LIB)
    file(GLOB GRAPHICS_SOURCES "${GRAPHICS_LIB}/*.cpp")
    if(BUILD_SHARED)
        add_library(graphics SHARED ${GRAPHICS_SOURCES})
        # On Windows, shared libraries need export macros for proper linking
        if(WIN32)
            target_compile_definitions(graphics PRIVATE GRAPHICS_BUILDING_DLL)
            target_compile_definitions(graphics INTERFACE GRAPHICS_USING_DLL)
        endif()
    else()
        add_library(graphics STATIC ${GRAPHICS_SOURCES})
    endif()
    target_include_directories(graphics PUBLIC ${INCLUDE_DIR})
    set_target_properties(graphics PROPERTIES OUTPUT_NAME "graphics")
    
    # Link SDL3 based on platform
    if(WIN32)
        if(SDL3_LIBRARY)
            target_link_libraries(graphics PRIVATE ${SDL3_LIBRARY})
            if(SDL3_MAIN_LIBRARY)
                target_link_libraries(graphics PRIVATE ${SDL3_MAIN_LIBRARY})
            endif()
        endif()
    else()
        if(SDL3_LIB)
            target_link_libraries(graphics PRIVATE ${SDL3_LIB})
        endif()
    endif()
    
    if(MATH_LIB)
        target_link_libraries(graphics PUBLIC ${MATH_LIB})
    endif()
endif()

if(BUILD_CALCX_EXE)
    file(GLOB CALCX_SOURCES "${CALCX_EXE}/*.cpp")
    add_executable(calcx ${CALCX_SOURCES})
    
    # Platform-specific linker flags for static linking
    if(NOT BUILD_SHARED)
        if(UNIX AND NOT APPLE)
            set(CMAKE_EXE_LINKER_FLAGS "-static")
        elseif(WIN32 AND MSVC)
            # On Windows with MSVC, use static runtime
            set_property(TARGET calcx PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        endif()
    endif()
    target_include_directories(calcx PRIVATE ${INCLUDE_DIR})
    
    # Add dependencies to ensure libraries are built first
    if(BUILD_CALC_LIB)
        add_dependencies(calcx calc)
        target_link_libraries(calcx PRIVATE calc)
    endif()
    
    if(BUILD_GRAPHICS_LIB)
        add_dependencies(calcx graphics)
        target_link_libraries(calcx PRIVATE graphics)
    endif()
    
    # Link SDL3 based on platform
    if(WIN32)
        if(SDL3_LIBRARY)
            target_link_libraries(calcx PRIVATE ${SDL3_LIBRARY})
            if(SDL3_MAIN_LIBRARY)
                target_link_libraries(calcx PRIVATE ${SDL3_MAIN_LIBRARY})
            endif()
        endif()
    else()
        if(SDL3_LIB)
            target_link_libraries(calcx PRIVATE ${SDL3_LIB})
        endif()
    endif()

    # Post-build message
    set(LINK_TYPE_MSG "static")
    if(BUILD_SHARED)
        set(LINK_TYPE_MSG "shared")
    endif()
    add_custom_command(TARGET calcx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Built calcx with ${LINK_TYPE_MSG} linking to calc library."
    )
    
    # Copy SDL3.dll to output directory if it exists
    if(WIN32 AND EXISTS "${CMAKE_SOURCE_DIR}/libraries/SDL3.dll")
        add_custom_command(TARGET calcx POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/libraries/SDL3.dll"
                $<TARGET_FILE_DIR:calcx>
            COMMENT "Copying SDL3.dll to output directory"
        )
    endif()
endif()
