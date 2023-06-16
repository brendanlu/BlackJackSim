import os
import shutil

# Cython build files
build_files = [
    r"cardstream.cp39-win_amd64.pyd", 
    r"cardstream\cardstream.cpp",
]

for file in build_files: 
    try:
        os.remove(file)
    except FileNotFoundError:
        print(file, "was not found.")

try:
    shutil.rmtree(r"build")
except FileNotFoundError:
    print("Build folder was not found.")