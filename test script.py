import cython

script_args = ["--cython-cplus"]
setup_args = {
    "script_args": script_args,
}

import pyximport
pyximport.install(setup_args=setup_args, language_level=3)

import testpyx

testpyx.some_func(5)
