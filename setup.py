from setuptools import Extension, setup
from Cython.Build import cythonize

import os

srcs = []
for file in os.listdir("src/"):
    if file.endswith(".cpp") and not file.startswith("test"):
        srcs.append("src/" + file)
srcs.append("cardstream/PySimEngineBJ.pyx")

extensions = [
    Extension(
        "PySimEngineBJ",
        sources=srcs,
        language="c++",
    )
]

print(srcs)

setup(ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}))
