import platform
from setuptools import Extension, setup
from Cython.Build import cythonize

import os

srcs = []
srcs.append("cardstream/_wrappers.pyx")
for file in os.listdir("cardstream/src/"):
    if file.endswith(".cpp") and not file.startswith("test"):
        srcs.append("cardstream/src/" + file)

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
        include_dirs=["cardstream/src", "e-socks/include"],
    )
]

setup(
    ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}),
    package_dir={"": "cardstream"},
)
