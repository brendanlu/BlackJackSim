from setuptools import Extension, setup
from Cython.Build import cythonize

import os

srcs = []
for file in os.listdir("src/"):
    if file.endswith(".cpp") and not file.startswith("test"):
        srcs.append("src/" + file)

for file in os.listdir("pyinterface/"):
    if file.endswith(".pyx"):
        srcs.append("pyinterface/" + file)

extensions = [
    Extension(
        "cardstream",
        sources=srcs,
        language="c++",
    )
]

print(srcs)

setup(ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}))
