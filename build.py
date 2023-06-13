# quick script to help make things easier for me
import os
import sys

if "-d" in sys.argv:
    print("Removing old build files")
    os.system("python clean.py")
elif "-b" in sys.argv:
    print("Building...")
    os.system("python setup.py build_ext --inplace")
else: 
    print("Removing files and rebuilding")
    os.system("python clean.py")
    os.system("python setup.py build_ext --inplace")

