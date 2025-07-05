@echo off
setlocal enabledelayedexpansion

:: Initialize variables
set BUILD_CALC_LIB=OFF
set BUILD_GRAPHICS_LIB=OFF
set BUILD_EXE=OFF
set BUILD_SHARED=OFF
set TEST=OFF
set BUILD_TYPE=Debug
set HELP=OFF

:: Parse command line arguments
:parse_args
if "%1"=="" goto :args_done
if /i "%1"=="exe" set BUILD_EXE=ON
if /i "%1"=="calc" set BUILD_CALC_LIB=ON
if /i "%1"=="graphics" set BUILD_GRAPHICS_LIB=ON
if /i "%1"=="static" set BUILD_SHARED=OFF
if /i "%1"=="shared" set BUILD_SHARED=ON
if /i "%1"=="test" set TEST=ON
if /i "%1"=="release" set BUILD_TYPE=Release
if /i "%1"=="help" set HELP=ON
shift
goto parse_args

:args_done

if "%HELP%"=="ON" (
    echo Usage: %0 [exe] [calc] [graphics] [static^|shared] [test] [release] [help]
    echo Options:
    echo   exe        Build the executable
    echo   calc       Build the calc library
    echo   graphics   Build the graphics library
    echo   static     Build static libraries ^(default^)
    echo   shared     Build shared libraries
    echo   test       Run tests after building
    echo   release    Build in release mode ^(default is debug^)
    echo   help       Show this help message
    exit /b 0
)

:: Set build directory
set BUILD_DIR=build\%BUILD_TYPE%
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Clean up CMake cache
if exist "%BUILD_DIR%\CMakeCache.txt" del "%BUILD_DIR%\CMakeCache.txt"
if exist "%BUILD_DIR%\CMakeFiles" rmdir /s /q "%BUILD_DIR%\CMakeFiles"

cd "%BUILD_DIR%"

:: Configure and build
echo cmake ..\.. -DBUILD_CALC_LIB=%BUILD_CALC_LIB% -DBUILD_GRAPHICS_LIB=%BUILD_GRAPHICS_LIB% -DBUILD_CALCX_EXE=%BUILD_EXE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_SHARED=%BUILD_SHARED%
cmake ..\.. -DBUILD_CALC_LIB=%BUILD_CALC_LIB% -DBUILD_GRAPHICS_LIB=%BUILD_GRAPHICS_LIB% -DBUILD_CALCX_EXE=%BUILD_EXE% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DBUILD_SHARED=%BUILD_SHARED%

if errorlevel 1 (
    echo CMake configuration failed!
    cd ..\..
    exit /b 1
)

echo -------------------------------------------
cmake --build . --config %BUILD_TYPE%

if errorlevel 1 (
    echo Build failed!
    cd ..\..
    exit /b 1
)

echo -------------------------------------------
cd ..\..

:: Set executable path based on build type and generator
set CALCX_DIR=build\%BUILD_TYPE%
if exist "%CALCX_DIR%\%BUILD_TYPE%\calcx.exe" (
    set CALCX=%CALCX_DIR%\%BUILD_TYPE%\calcx.exe
) else if exist "%CALCX_DIR%\calcx.exe" (
    set CALCX=%CALCX_DIR%\calcx.exe
) else (
    set CALCX=
)

:: Run tests if requested
if "%BUILD_EXE%"=="ON" if "%TEST%"=="ON" if not "%CALCX%"=="" (
    echo Running %CALCX%
    
    if "%BUILD_GRAPHICS_LIB%"=="ON" (
        echo -------------------------------------------
        "%CALCX%" --graphics
        echo -----------------xxx-----------------------
    )
    
    if "%BUILD_CALC_LIB%"=="ON" (
        echo -------------------------------------------
        "%CALCX%" --calc
        echo -----------------xxx-----------------------
    )
    
    echo -------------------------------------------
    "%CALCX%" --version
    echo -----------------xxx-----------------------
    dir "%CALCX%"
    echo -------------------------------------------
    echo Executable %CALCX% executed successfully.
    echo Build and execution completed successfully.
) else (
    if "%BUILD_EXE%"=="ON" if "%TEST%"=="ON" (
        echo Build failed or executable not found.
    ) else (
        echo Executable not built or testing not requested. Skipping execution.
    )
)

endlocal
