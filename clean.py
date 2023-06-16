import os
import shutil

# Cython build files
build_files = [
    r"cardstream.cp39-win_amd64.pyd", # python 3.9
    r"cardstream.cp310-win_amd64.pyd", # python 3.10
    r"cardstream\cardstream.cpp",
]

for file in build_files: 
    try:
        os.remove(file)
        print("Removed", file)
    except FileNotFoundError:
        print(file, "was not found.")

try:
    shutil.rmtree(r"build")
    print("Removed build folder.")
except FileNotFoundError:
    print("Build folder was not found.")

