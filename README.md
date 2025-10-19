# Boot Security Parameter Checker

A Windows GUI application that checks important boot and security parameters on your system.

## Features

This application checks the following boot and security parameters:

- **UEFI Mode**: Verifies if the system is running in UEFI or Legacy BIOS mode
- **Secure Boot**: Checks if Secure Boot is enabled
- **TPM (Trusted Platform Module)**: Detects TPM presence and version
- **Virtualization-Based Security (VBS)**: Checks if VBS is enabled
- **BitLocker**: Verifies BitLocker encryption status
- **Windows Defender**: Checks Tamper Protection status

## Building the Application

### Quick Build - Production Ready (Recommended)

The easiest way to build a production version with all dependencies:

#### Using PowerShell:
```powershell
# Simple production build
.\build_production.ps1

# Clean build with deployment package
.\deploy.ps1 -Clean
```

#### Using Batch Script:
```batch
build_production.bat
```

All production files will be output to the `/final/` directory, including:
- `BootCheck.exe` - Main application executable
- Required DLL dependencies (if any)
- `README.txt` - Build information

### Manual CMake Build

```batch
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cmake --install . --config Release
```

### Using Visual Studio

1. Open Developer Command Prompt for Visual Studio
2. Navigate to the project directory
3. Run:
```batch
cl /EHsc /W3 main.cpp /link comctl32.lib tbs.lib user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
```

### Using MinGW

```batch
g++ -municode -mwindows main.cpp -o BootCheck.exe -lcomctl32 -ltbs -lgdi32
```

## Deployment

To create a distributable ZIP package:

```powershell
.\deploy.ps1
```

This will build the application and create a timestamped ZIP file ready for distribution.

## Output Directory Structure

```
boot-check/
??? final/                  # Production output (ready for deployment)
?   ??? BootCheck.exe      # Main executable
?   ??? *.dll              # Runtime dependencies (if needed)
?   ??? README.txt         # Build information
??? build/                 # CMake build directory
??? BootCheck_YYYY-MM-DD.zip  # Distribution package
```

## Usage

1. Run the `BootCheck.exe` executable (from the `/final/` directory)
2. The application will automatically check all boot parameters on startup
3. Click the "Check Boot Parameters" button to refresh the results
4. Review the security status of your system

## Requirements

- Windows 10 or later (Windows 11 recommended)
- Administrator privileges recommended for accurate checks
- TPM hardware (for TPM checks)
- UEFI firmware (for full functionality)

## Security Parameters Explained

### Secure Boot
Secure Boot helps protect your system from malware by only allowing trusted operating systems to boot. **Status: Enabled** is recommended for maximum security.

### TPM
Trusted Platform Module is a hardware security chip that provides cryptographic functions. Required for Windows 11 and BitLocker.

### UEFI Mode
UEFI is the modern replacement for Legacy BIOS and provides better security features including Secure Boot support.

### Virtualization-Based Security (VBS)
VBS uses hardware virtualization to create isolated regions of memory, protecting critical system components from malware.

### BitLocker
Full disk encryption that protects your data if your device is lost or stolen.

## Technical Details

### Build System
- **CMake Version**: 3.31.6
- **Minimum CMake**: 3.10
- **C++ Standard**: C++11
- **Generator**: Ninja
- **Build Type**: Release (for production)

### Dependencies
- `comctl32.lib` - Common Controls (part of Windows)
- `tbs.lib` - TPM Base Services (part of Windows)

Note: System DLLs are part of Windows and don't need to be distributed.

## Note

Some features may show as "UNKNOWN" if:
- The feature is not configured on your system
- You need administrator privileges to read certain registry keys
- The feature is not available on your Windows version

## Troubleshooting

### Build Issues
- **CMake not found**: Install CMake from https://cmake.org/download/
- **Ninja not found**: Install via `choco install ninja` or download from GitHub
- **Missing Windows SDK**: Install Visual Studio with Windows SDK component

### Runtime Issues
- **Unknown results**: Try running as Administrator
- **Application won't start**: Ensure Visual C++ Redistributables are installed

## License

This is free and open-source software.
