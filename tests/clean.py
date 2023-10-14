"""
This clunky script will clean both this directory and the python module 
after building and test-runs. 
"""

import os
import shutil

MODULE_DIR = "../cardstream/"

# remove build folder
try:
    shutil.rmtree(MODULE_DIR + "build")
    print("Removed build folder")
except FileNotFoundError:
    pass

# Cython generated C++ file
try:
    os.remove(MODULE_DIR + "_wrappers.cpp")
except FileNotFoundError:
    pass

# remove cython generated files, and compiled extensions
for file in os.listdir(MODULE_DIR):
    if file.endswith(".pyd"):
        os.remove(MODULE_DIR + file)
        print("Removed", file)

# clean from a simulation run
for file in os.listdir():
    if "LOG" in file or "ERROR" in file:
        os.remove(file)
        print("Removed", file)
