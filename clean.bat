@echo off
REM Clean script - Removes all build artifacts

echo ========================================
echo Boot Security Parameter Checker
echo Clean Script
echo ========================================
echo.

set CLEANED=0

REM Remove build directory
if exist "build" (
    echo Removing build directory...
    rmdir /s /q "build"
    set CLEANED=1
)

REM Remove final directory
if exist "final" (
    echo Removing final directory...
    rmdir /s /q "final"
    set CLEANED=1
)

REM Remove distribution packages
if exist "BootCheck_*.zip" (
    echo Removing distribution packages...
    del /q "BootCheck_*.zip"
    set CLEANED=1
)

REM Remove CMake generated files
if exist "CMakeCache.txt" (
    echo Removing CMake cache...
    del /q "CMakeCache.txt"
    set CLEANED=1
)

if exist "cmake_install.cmake" (
    del /q "cmake_install.cmake"
    set CLEANED=1
)

if exist "CMakeFiles" (
    rmdir /s /q "CMakeFiles"
    set CLEANED=1
)

echo.
if %CLEANED%==1 (
    echo [32mProject cleaned successfully![0m
) else (
    echo [32mProject is already clean![0m
)
echo.
pause
