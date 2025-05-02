# Name        : export-dll-functions-stubs.py
# Description : Extract the exported function names from a DLL file and generate the function stubs.  
#               Add the script output at the end of the payload file before compiling it.
# Examples    : python3 export-dll-functions-stubs.py version.dll > version-stubs.txt
#               cat reverse.c version-stubs.txt > version.c
# Prereq      : pip install pefile
# Author      : Lisandre.com
# Date        : 2025-05-02  
import pefile
import sys

pe = pefile.PE(sys.argv[1])

if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        if exp.name:
            name = exp.name.decode()
            print(f'__declspec(dllexport) void {name}() {{}}')
else:
    print("No exports found.")
