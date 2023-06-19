from setuptools import Extension, setup
from Cython.Build import cythonize

import os

srcs = []
for file in os.listdir("src/"): 
    if file.endswith(".cpp"): 
        srcs.append(file)

extensions = [
    Extension("cardstream", srcs.append("cardstream/cardstream.pyx"))
]

print(srcs)

setup(
    ext_modules = cythonize(
        extensions
    )
)
