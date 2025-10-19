#!/bin/bash
# Build script for Boot Security Parameter Checker - Production Build

echo "========================================"
echo "Boot Security Parameter Checker"
echo "Production Build Script"
echo "========================================"
echo ""

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

echo "Configuring CMake..."
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed!"
    cd ..
    exit 1
fi

echo ""
echo "Building project..."
cmake --build . --config Release
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    cd ..
    exit 1
fi

echo ""
echo "Installing runtime dependencies..."
cmake --install . --config Release

cd ..

echo ""
echo "========================================"
echo "Build Complete!"
echo "========================================"
echo ""
echo "Production files are located in: $(pwd)/final/"
echo ""
echo "Contents:"
ls -la final/
echo ""
