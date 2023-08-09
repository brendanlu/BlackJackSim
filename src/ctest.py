"""
Simple clunky script to automate cpp compilation, and run, for testing.
"""

import os
import sys
import time

for file in os.listdir():
    if file.endswith(".o") or file.endswith(".exe"):
        os.remove(file)
        print("Removed", file)

# can pass -d flag for just delete, and not this next bit
if "-d" not in sys.argv:
    print("Compiling testcpp")
    os.system("make -f Makefile.mk")
    # let it compile - hopefully
    time.sleep(1.3)
    if "test.exe" in os.listdir():
        print("Found file; output-------------------------------------------")
        os.system("test.exe")
    else:
        print("WAS UNABLE TO FIND EXE FILE IN TIME")
