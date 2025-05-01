# Name        : export-dll-functions.py
# Description : Extract the exported function names from a DLL file.  
#               Generates output in a .def file format.
# Examples    : python3 export-dll-functions.py msasn1.dll > msasn1.def
# Prereq      : pip install pefile
# Author      : Lisandre.com
# Date        : 2025-05-01  
import pefile
import sys

pe = pefile.PE(sys.argv[1])
print("EXPORTS")
for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
    if exp.name:
        print(exp.name.decode())
