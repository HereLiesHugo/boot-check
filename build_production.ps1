# Build script for Boot Security Parameter Checker - Production Build
# PowerShell version

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Boot Security Parameter Checker" -ForegroundColor Cyan
Write-Host "Production Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
    Write-Host "Created build directory" -ForegroundColor Green
}

# Navigate to build directory
Push-Location build

try {
    Write-Host "Configuring CMake..." -ForegroundColor Yellow
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed!"
    }

    Write-Host ""
    Write-Host "Building project..." -ForegroundColor Yellow
    cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed!"
    }

    Write-Host ""
    Write-Host "Installing runtime dependencies..." -ForegroundColor Yellow
    cmake --install . --config Release

    Pop-Location

    Write-Host ""
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "Build Complete!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Production files are located in: $PWD\final\" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Contents:" -ForegroundColor Cyan
    Get-ChildItem -Path "final" | Format-Table Name, Length, LastWriteTime
    Write-Host ""
    
    # Check file size
    $exePath = "final\BootCheck.exe"
    if (Test-Path $exePath) {
        $fileSize = (Get-Item $exePath).Length
        $fileSizeKB = [math]::Round($fileSize / 1KB, 2)
        Write-Host "BootCheck.exe size: $fileSizeKB KB" -ForegroundColor Green
    }
    
} catch {
    Pop-Location
    Write-Host ""
    Write-Host "ERROR: $_" -ForegroundColor Red
    exit 1
}
