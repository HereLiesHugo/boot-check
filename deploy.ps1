# Deploy script - Creates production build and packages it for distribution

param(
    [switch]$Clean = $false
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Boot Security Parameter Checker" -ForegroundColor Cyan
Write-Host "Deployment Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Clean previous builds if requested
if ($Clean) {
    Write-Host "Cleaning previous builds..." -ForegroundColor Yellow
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
        Write-Host "  - Removed build directory" -ForegroundColor Gray
    }
    if (Test-Path "final") {
        Remove-Item -Recurse -Force "final"
        Write-Host "  - Removed final directory" -ForegroundColor Gray
    }
    Write-Host ""
}

# Create final directory if it doesn't exist
if (-not (Test-Path "final")) {
    New-Item -ItemType Directory -Path "final" | Out-Null
}

# Run the production build
Write-Host "Starting production build..." -ForegroundColor Yellow
& .\build_production.ps1
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Creating Distribution Package" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Get version/date for package name
$date = Get-Date -Format "yyyy-MM-dd"
$packageName = "BootCheck_$date.zip"

# Create package
Write-Host "Creating ZIP package: $packageName" -ForegroundColor Yellow
$source = "final\*"
$destination = $packageName

if (Test-Path $destination) {
    Remove-Item $destination -Force
}

Compress-Archive -Path $source -DestinationPath $destination -CompressionLevel Optimal

if (Test-Path $destination) {
    $zipSize = (Get-Item $destination).Length
    $zipSizeKB = [math]::Round($zipSize / 1KB, 2)
    Write-Host "Package created successfully! Size: $zipSizeKB KB" -ForegroundColor Green
} else {
    Write-Host "Failed to create package!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "Deployment Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Distribution package: $PWD\$packageName" -ForegroundColor Cyan
Write-Host "Production files: $PWD\final\" -ForegroundColor Cyan
Write-Host ""
Write-Host "Ready for deployment!" -ForegroundColor Green
Write-Host ""
