import platform
from setuptools import Extension, setup
from Cython.Build import cythonize

import os

# ensure this works even when being invoked from different locations
# NOTE: not fully working as temp build folder still gets placed incorrectly
SETUP_PATH = os.path.dirname(os.path.abspath(__file__))
SRC_PATH = os.path.join(SETUP_PATH, "cardstream", "src")
E_SOCKS_INCLUDE_PATH = os.path.join(SETUP_PATH, "e-socks", "include")

srcs = []

srcs.append(os.path.join(SETUP_PATH, "cardstream", "_wrappers.pyx"))

for file in os.listdir(SRC_PATH):
    if file.endswith(".cpp") and not file.startswith("test"):
        srcs.append(os.path.join(SRC_PATH, file))

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
        include_dirs=[SRC_PATH, E_SOCKS_INCLUDE_PATH],
    )
]

setup(
    ext_modules=cythonize(
        extensions,
        compiler_directives={"language_level": "3"},
    ),
    package_dir={"": "cardstream"},  # place it in the cardstream directory
)
