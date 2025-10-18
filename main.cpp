// Windows Boot Parameter Checker
// Checks Secure Boot, TPM, UEFI, and other boot parameters

#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <commctrl.h>
#include <tbs.h>
#include <string>
#include <sstream>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "tbs.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Window dimensions
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 500

// Control IDs
#define ID_REFRESH_BUTTON 1001
#define ID_RESULTS_EDIT 1002

// Global variables
HWND g_hMainWindow = NULL;
HWND g_hResultsEdit = NULL;
HWND g_hRefreshButton = NULL;
HFONT g_hFont = NULL;

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::wstring CheckSecureBoot();
std::wstring CheckTPM();
std::wstring CheckUEFI();
std::wstring CheckVirtualizationBasedSecurity();
std::wstring CheckBitLocker();
std::wstring CheckWindowsDefender();
void PerformChecks();
void AppendText(const std::wstring& text);

// Check if Secure Boot is enabled
std::wstring CheckSecureBoot() {
    DWORD secureBootEnabled = 0;
    DWORD size = sizeof(DWORD);
    
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
        L"SYSTEM\\CurrentControlSet\\Control\\SecureBoot\\State", 
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        if (RegQueryValueExW(hKey, L"UEFISecureBootEnabled", NULL, NULL, 
            (LPBYTE)&secureBootEnabled, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return secureBootEnabled ? L"? ENABLED" : L"? DISABLED";
        }
        RegCloseKey(hKey);
    }
    return L"? UNKNOWN (Unable to read registry)";
}

// Check TPM status
std::wstring CheckTPM() {
    TBS_CONTEXT_PARAMS2 contextParams;
    contextParams.version = TBS_CONTEXT_VERSION_TWO;
    contextParams.includeTpm12 = 0;
    contextParams.includeTpm20 = 1;
    
    TBS_HCONTEXT hContext;
    TBS_RESULT result = Tbsi_Context_Create((PCTBS_CONTEXT_PARAMS)&contextParams, &hContext);
    
    if (result == TBS_SUCCESS) {
        TPM_DEVICE_INFO deviceInfo;
        result = Tbsi_GetDeviceInfo(sizeof(deviceInfo), &deviceInfo);
        Tbsip_Context_Close(hContext);
        
        if (result == TBS_SUCCESS) {
            std::wstringstream ss;
            ss << L"? PRESENT (TPM Version: " << deviceInfo.tpmVersion << L")";
            return ss.str();
        }
    }
    
    if (result == TBS_E_TPM_NOT_FOUND) {
        return L"? NOT FOUND";
    }
    
    return L"? UNKNOWN (Unable to query TPM)";
}

// Check if system is running in UEFI mode
std::wstring CheckUEFI() {
    FIRMWARE_TYPE firmwareType;
    BOOL result = GetFirmwareType(&firmwareType);
    
    if (result) {
        switch (firmwareType) {
            case FirmwareTypeUefi:
                return L"? UEFI Mode";
            case FirmwareTypeBios:
                return L"? Legacy BIOS Mode";
            default:
                return L"? UNKNOWN Firmware Type";
        }
    }
    return L"? UNKNOWN (Unable to determine)";
}

// Check Virtualization-Based Security (VBS)
std::wstring CheckVirtualizationBasedSecurity() {
    DWORD vbsEnabled = 0;
    DWORD size = sizeof(DWORD);
    
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
        L"SYSTEM\\CurrentControlSet\\Control\\DeviceGuard", 
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        if (RegQueryValueExW(hKey, L"EnableVirtualizationBasedSecurity", NULL, NULL, 
            (LPBYTE)&vbsEnabled, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return vbsEnabled ? L"? ENABLED" : L"? DISABLED";
        }
        RegCloseKey(hKey);
    }
    return L"? UNKNOWN or NOT CONFIGURED";
}

// Check BitLocker status
std::wstring CheckBitLocker() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
        L"SYSTEM\\CurrentControlSet\\Control\\BitLockerStatus", 
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        DWORD bootStatus = 0;
        DWORD size = sizeof(DWORD);
        if (RegQueryValueExW(hKey, L"BootStatus", NULL, NULL, 
            (LPBYTE)&bootStatus, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return bootStatus ? L"? ENABLED" : L"? DISABLED";
        }
        RegCloseKey(hKey);
    }
    return L"? UNKNOWN or NOT CONFIGURED";
}

// Check Windows Defender status
std::wstring CheckWindowsDefender() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, 
        L"SOFTWARE\\Microsoft\\Windows Defender\\Features", 
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        DWORD tamperProtection = 0;
        DWORD size = sizeof(DWORD);
        if (RegQueryValueExW(hKey, L"TamperProtection", NULL, NULL, 
            (LPBYTE)&tamperProtection, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return tamperProtection ? L"? TAMPER PROTECTION ENABLED" : L"? TAMPER PROTECTION DISABLED";
        }
        RegCloseKey(hKey);
    }
    return L"? UNKNOWN";
}

// Append text to the results edit control
void AppendText(const std::wstring& text) {
    int length = GetWindowTextLengthW(g_hResultsEdit);
    SendMessageW(g_hResultsEdit, EM_SETSEL, length, length);
    SendMessageW(g_hResultsEdit, EM_REPLACESEL, FALSE, (LPARAM)text.c_str());
}

// Perform all security checks
void PerformChecks() {
    SetWindowTextW(g_hResultsEdit, L"");
    
    AppendText(L"====================================\r\n");
    AppendText(L"  BOOT SECURITY PARAMETER CHECKER\r\n");
    AppendText(L"====================================\r\n\r\n");
    
    AppendText(L"[FIRMWARE]\r\n");
    AppendText(L"  UEFI Mode: " + CheckUEFI() + L"\r\n\r\n");
    
    AppendText(L"[SECURE BOOT]\r\n");
    AppendText(L"  Status: " + CheckSecureBoot() + L"\r\n\r\n");
    
    AppendText(L"[TPM (Trusted Platform Module)]\r\n");
    AppendText(L"  Status: " + CheckTPM() + L"\r\n\r\n");
    
    AppendText(L"[VIRTUALIZATION-BASED SECURITY]\r\n");
    AppendText(L"  VBS Status: " + CheckVirtualizationBasedSecurity() + L"\r\n\r\n");
    
    AppendText(L"[BITLOCKER]\r\n");
    AppendText(L"  Status: " + CheckBitLocker() + L"\r\n\r\n");
    
    AppendText(L"[WINDOWS DEFENDER]\r\n");
    AppendText(L"  Status: " + CheckWindowsDefender() + L"\r\n\r\n");
    
    AppendText(L"====================================\r\n");
    AppendText(L"  Check completed!\r\n");
    AppendText(L"====================================\r\n");
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Create font
            g_hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
            
            // Create refresh button
            g_hRefreshButton = CreateWindowW(
                L"BUTTON", L"Check Boot Parameters",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                20, 20, 200, 40,
                hwnd, (HMENU)ID_REFRESH_BUTTON, NULL, NULL
            );
            SendMessageW(g_hRefreshButton, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            
            // Create results edit control
            g_hResultsEdit = CreateWindowExW(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_VISIBLE | WS_CHILD | WS_VSCROLL | 
                ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
                20, 80, WINDOW_WIDTH - 60, WINDOW_HEIGHT - 130,
                hwnd, (HMENU)ID_RESULTS_EDIT, NULL, NULL
            );
            SendMessageW(g_hResultsEdit, WM_SETFONT, (WPARAM)g_hFont, TRUE);
            
            // Initial check
            PerformChecks();
            break;
        }
        
        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_REFRESH_BUTTON) {
                PerformChecks();
            }
            break;
        }
        
        case WM_DESTROY: {
            if (g_hFont) DeleteObject(g_hFont);
            PostQuitMessage(0);
            break;
        }
        
        case WM_SIZE: {
            // Resize controls when window is resized
            RECT rect;
            GetClientRect(hwnd, &rect);
            MoveWindow(g_hResultsEdit, 20, 80, 
                rect.right - 40, rect.bottom - 100, TRUE);
            break;
        }
        
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// WinMain - Application entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    PWSTR pCmdLine, int nCmdShow) {
    
    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);
    
    // Register window class
    const wchar_t CLASS_NAME[] = L"BootCheckWindowClass";
    
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassW(&wc);
    
    // Create window
    g_hMainWindow = CreateWindowExW(
        0, CLASS_NAME, L"Boot Security Parameter Checker",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );
    
    if (g_hMainWindow == NULL) {
        return 0;
    }
    
    ShowWindow(g_hMainWindow, nCmdShow);
    UpdateWindow(g_hMainWindow);
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}
