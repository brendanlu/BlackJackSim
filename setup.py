from setuptools import Extension, setup
from Cython.Build import cythonize

import os

os.environ['CC'] = 'g++'
os.environ['CXX'] = 'g++'

srcs = []
for file in os.listdir("src/"): 
    if file.endswith(".cpp"): 
        srcs.append("src/" + file)
srcs.append("cardstream/cardstream.pyx")

extensions = [
    Extension(
        "cardstream", 
        sources = srcs, 
        language = "c++",
        extra_compile_args=["-o3"]
    )
]

print(srcs)

setup(
    ext_modules = cythonize(
        extensions,
        compiler_directives={'language_level' : "3"}
    )
)
