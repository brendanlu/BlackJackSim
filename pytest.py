# quick script to help make things easier for me
import os
import sys

if "-d" in sys.argv:
    print("Removing old build files")
    os.system("python clean.py")
else: 
    print("Removing files and rebuilding")
    os.system("python clean.py")
    os.system("python setup.py build_ext --inplace")
    print("\nTesting in python-------------------------------------")
    os.system("python callfunc.py")
