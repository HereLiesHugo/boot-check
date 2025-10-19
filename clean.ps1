# Clean script - Removes all build artifacts and output files

param(
    [switch]$KeepFinal = $false
)

Write-Host "========================================" -ForegroundColor Yellow
Write-Host "Boot Security Parameter Checker" -ForegroundColor Yellow
Write-Host "Clean Script" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow
Write-Host ""

$cleaned = $false

# Remove build directory
if (Test-Path "build") {
    Write-Host "Removing build directory..." -ForegroundColor Gray
    Remove-Item -Recurse -Force "build"
    $cleaned = $true
}

# Remove final directory (unless -KeepFinal is specified)
if (-not $KeepFinal) {
    if (Test-Path "final") {
        Write-Host "Removing final directory..." -ForegroundColor Gray
        Remove-Item -Recurse -Force "final"
        $cleaned = $true
    }
}

# Remove distribution packages
$zipFiles = Get-ChildItem -Filter "BootCheck_*.zip" -ErrorAction SilentlyContinue
if ($zipFiles) {
    Write-Host "Removing distribution packages..." -ForegroundColor Gray
    $zipFiles | Remove-Item -Force
    $cleaned = $true
}

# Remove CMake generated files if they exist in root
$cmakeFiles = @(
    "CMakeCache.txt",
    "cmake_install.cmake",
    "CMakeFiles"
)

foreach ($file in $cmakeFiles) {
    if (Test-Path $file) {
        Write-Host "Removing $file..." -ForegroundColor Gray
        Remove-Item -Recurse -Force $file -ErrorAction SilentlyContinue
        $cleaned = $true
    }
}

Write-Host ""
if ($cleaned) {
    Write-Host "? Project cleaned successfully!" -ForegroundColor Green
    if ($KeepFinal) {
        Write-Host "  (final directory preserved)" -ForegroundColor Cyan
    }
} else {
    Write-Host "? Project is already clean!" -ForegroundColor Green
}
Write-Host ""
