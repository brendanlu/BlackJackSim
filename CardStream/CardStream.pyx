# distutils: language = c++

import numpy as np
cimport SimEngine

# Create a packed struct in C representing one card in the shoe
# https://cython.readthedocs.io/en/latest/src/userguide/memoryviews.html
cdef packed struct card_dtype_struct: 
    char face
    char suit

def some_func(card_dtype_struct[:] shoe): 
    return SimEngine.bounce_back(a)

