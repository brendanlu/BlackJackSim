# include cpp code
# https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
cdef extern from "../src/simenginebj.cpp": 
    pass

# include header file stuff 
cdef extern from "../src/simenginebj.hpp":
    cdef cppclass SimEngineBJ: 
        SimEngine(unsigned int, double, char*, char*, char*, double*) except + 
        char Test()
