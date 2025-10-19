cmake_minimum_required(VERSION 3.10)
project(BootParameterChecker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory to /final/ for production builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/final)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/final)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/final)

# Add executable
add_executable(BootCheck WIN32 main.cpp)

# Link required Windows libraries
target_link_libraries(BootCheck 
    comctl32
    tbs
)

# Set subsystem to Windows (GUI application)
if(MSVC)
    set_target_properties(BootCheck PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()

# Post-build: Copy required DLL dependencies to final directory
# Note: System DLLs (comctl32.dll, tbs.dll) are part of Windows and don't need to be copied
# If using MinGW or other compiler that requires runtime DLLs, add them here

# For MSVC, copy the Visual C++ runtime DLLs if needed
if(MSVC)
    # Copy runtime dependencies using CMake's install feature
    install(TARGETS BootCheck
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/final
    )
    
    # Install system runtime libraries
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
    include(InstallRequiredSystemLibraries)
    if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
        install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
            DESTINATION ${CMAKE_SOURCE_DIR}/final
            COMPONENT System
        )
    endif()
endif()

# For MinGW, copy MinGW runtime DLLs
if(MINGW)
    # Get the compiler path
    get_filename_component(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
    
    # Common MinGW DLLs that might be needed
    set(MINGW_DLLS
        libgcc_s_seh-1.dll
        libstdc++-6.dll
        libwinpthread-1.dll
    )
    
    foreach(DLL ${MINGW_DLLS})
        if(EXISTS "${MINGW_BIN_PATH}/${DLL}")
            add_custom_command(TARGET BootCheck POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MINGW_BIN_PATH}/${DLL}"
                ${CMAKE_SOURCE_DIR}/final/
                COMMENT "Copying ${DLL} to final directory"
            )
        endif()
    endforeach()
endif()

# Create a README in the final directory
file(WRITE ${CMAKE_SOURCE_DIR}/final/README.txt 
"Boot Security Parameter Checker - Production Build
==================================================

This directory contains the production-ready executable and all required dependencies.

Files:
- BootCheck.exe: Main application executable
- *.dll: Runtime library dependencies (if any)

System Requirements:
- Windows 10 or later
- UEFI firmware (for full functionality)
- Administrator privileges (recommended for accurate security checks)

Note: Some DLLs (comctl32.dll, tbs.dll) are part of Windows and are not included here.

Built with: CMake ${CMAKE_VERSION}
Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}
")
