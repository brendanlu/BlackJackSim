from setuptools import Extension, setup
from Cython.Build import cythonize

import os

srcs = []
for file in os.listdir("src/"):
    if file.endswith(".cpp"):
        srcs.append("src/" + file)
srcs.append("cardstream/cardstream.pyx")

extensions = [
    Extension(
        "cardstream",
        sources=srcs,
        language="c++",
    )
]

print(srcs)

setup(ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}))
