"""
Simple clunky script to automate cpp compilation, and run, for testing.
"""

import os
import sys
import time


os.system("make -f Makefile.mk clean")

# can pass -d flag for just delete, and not this next bit
if "-d" not in sys.argv:
    print("Compiling testcpp")
    os.system("make -f Makefile.mk")
    # let it compile - hopefully
    time.sleep(1)
    if "test.exe" in os.listdir():
        print("Found file; output-------------------------------------------")
        os.system("test.exe")
    else: 
        print("WAS UNABLE TO FIND EXE FILE")
