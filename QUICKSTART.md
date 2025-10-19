# Quick Start Guide

## For End Users

### Running the Application
1. Navigate to the `/final/` directory
2. Double-click `BootCheck.exe`
3. The application will display your system's boot security parameters

**Note**: For accurate results, run as Administrator (right-click ? "Run as administrator")

---

## For Developers

### Building Production Version

#### Quick Build (Recommended):
```powershell
.\build_production.ps1
```

#### Create Distribution Package:
```powershell
.\deploy.ps1
```

#### Clean Build:
```powershell
.\clean.ps1
.\build_production.ps1
```

### Output Location
All production files are in: `/final/`
- `BootCheck.exe` - Main executable (31 KB)
- `*.dll` files - Visual C++ Runtime libraries (~1.15 MB total)
- `README.txt` - Build information

### Distribution Package
The `deploy.ps1` script creates a ZIP file named `BootCheck_YYYY-MM-DD.zip` containing all files from `/final/`.

Total distribution size: ~1.18 MB

---

## Build Scripts

| Script | Purpose |
|--------|---------|
| `build_production.ps1` | PowerShell build script (recommended) |
| `build_production.bat` | Windows batch build script |
| `deploy.ps1` | Build and create ZIP package |
| `clean.ps1` / `clean.bat` | Remove build artifacts |

---

## Manual Build

```cmd
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cmake --install . --config Release
cd ..
```

Production files will be in `/final/` directory.

---

## System Requirements

### Build Requirements:
- CMake 3.10+
- C++11 compiler (MSVC, MinGW, or Clang)
- Ninja build system
- Windows SDK

### Runtime Requirements:
- Windows 10 or later
- Visual C++ Redistributables (included in `/final/`)

---

## Troubleshooting

**Q: Build fails with "CMake not found"**
A: Install CMake from https://cmake.org/download/ or via `choco install cmake`

**Q: Build fails with "Ninja not found"**
A: Install Ninja via `choco install ninja` or download from GitHub

**Q: Application shows "UNKNOWN" for all checks**
A: Run as Administrator for accurate results

**Q: Missing DLL errors when running**
A: Make sure all DLL files from `/final/` are in the same directory as `BootCheck.exe`

---

## File Structure

```
boot-check/
??? main.cpp                    # Source code
??? CMakeLists.txt              # CMake configuration
??? README.md                   # Full documentation
??? QUICKSTART.md              # This file
?
??? build_production.ps1        # PowerShell build script ?
??? build_production.bat        # Batch build script
??? build_production.sh         # Unix build script
??? deploy.ps1                  # Deployment script
??? clean.ps1 / clean.bat      # Clean scripts
?
??? build/                      # CMake build directory (generated)
?
??? final/                      # Production output ?
    ??? BootCheck.exe           # Main executable
    ??? *.dll                   # Runtime dependencies
    ??? README.txt              # Build info
```

---

## Need Help?

See `README.md` for comprehensive documentation.
