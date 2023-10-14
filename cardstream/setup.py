import platform
from setuptools import Extension, setup
from Cython.Build import cythonize

import os

srcs = []
srcs.append("_wrappers.pyx")
for file in os.listdir("src/"):
    if file.endswith(".cpp") and not file.startswith("test"):
        srcs.append("src/" + file)

libs = []
if platform.system() == "Windows":
    # link winsock
    libs.append("ws2_32")

extensions = [
    Extension(
        "_wrappers",
        sources=srcs,
        language="c++",
        libraries=libs,
        include_dirs=["src", "../e-socks/include"],
    )
]

setup(ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}))
