cmake_minimum_required(VERSION 3.30)
project(calc-lib-linking C CXX)

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


set(SDL3_LIB "${CMAKE_SOURCE_DIR}/libraries/libSDL3.so.0")

if(BUILD_CALC_LIB)
    file(GLOB CALC_SOURCES "${CALC_LIB}/*.cpp")
    if(BUILD_SHARED)
        add_library(calc SHARED ${CALC_SOURCES})
    else()
        add_library(calc STATIC ${CALC_SOURCES})
    endif()
    target_include_directories(calc PUBLIC ${INCLUDE_DIR})
    set_target_properties(calc PROPERTIES OUTPUT_NAME "calc")
    target_link_libraries(calc PUBLIC m)
endif()

if(BUILD_GRAPHICS_LIB)
    file(GLOB GRAPHICS_SOURCES "${GRAPHICS_LIB}/*.cpp")
    if(BUILD_SHARED)
        add_library(graphics SHARED ${GRAPHICS_SOURCES})
    else()
        add_library(graphics STATIC ${GRAPHICS_SOURCES})
    endif()
    target_include_directories(graphics PUBLIC ${INCLUDE_DIR})
    set_target_properties(graphics PROPERTIES OUTPUT_NAME "graphics")
    target_link_libraries(graphics PRIVATE ${SDL3_LIB})
    target_link_libraries(graphics PUBLIC m)
endif()

if(BUILD_CALCX_EXE)
    if(NOT BUILD_SHARED)
        set(CMAKE_EXE_LINKER_FLAGS "-static")
    endif()
    file(GLOB CALCX_SOURCES "${CALCX_EXE}/*.cpp")
    add_executable(calcx ${CALCX_SOURCES})
    target_include_directories(calcx PRIVATE ${INCLUDE_DIR})
    target_link_libraries(calcx PRIVATE calc)
    target_link_libraries(calcx PRIVATE ${SDL3_LIB})
    target_link_libraries(calcx PRIVATE graphics)

    # Post-build message
    set(LINK_TYPE_MSG "static")
    if(BUILD_SHARED)
        set(LINK_TYPE_MSG "shared")
    endif()
    add_custom_command(TARGET calcx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Built calcx with ${LINK_TYPE_MSG} linking to calc library."
    )
endif()
