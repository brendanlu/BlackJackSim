# include cpp code
# https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
cdef extern from "src/SimEngine.cpp": 
    pass

# include header file stuff 
cdef extern from "src/SimEngine.hpp":
    int bounce_back(int a)

