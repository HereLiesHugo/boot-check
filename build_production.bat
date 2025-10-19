@echo off
REM Build script for Boot Security Parameter Checker - Production Build

echo ========================================
echo Boot Security Parameter Checker
echo Production Build Script
echo ========================================
echo.

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Navigate to build directory
cd build

echo Configuring CMake...
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo Installing runtime dependencies...
cmake --install . --config Release

cd ..

echo.
echo ========================================
echo Build Complete!
echo ========================================
echo.
echo Production files are located in: %CD%\final\
echo.
echo Contents:
dir /B final\
echo.
pause
