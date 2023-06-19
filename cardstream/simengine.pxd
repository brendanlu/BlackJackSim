
"""
# include cpp code
# https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
cdef extern from "../src/simenginebj.cpp": 
    pass
"""
"""
BECAUSE WE ACTUALLY LIST THE CPP FILE AS A SOURCE, CYTHON CAN FIND IT VIA THE COMPILER LINKER. 
THE ONLINE DOCS ONLY INCLUDE THIS AS A SIMPLE EXAMPLE, BUT IN OUR CASE, INCLUDING IT IS LIKE INCLUDING CPP FILES IN CPP CODE.
"""

# include header file stuff 
cdef extern from "../src/simenginebj.hpp":
    cdef cppclass SimEngineBJ: 
        SimEngineBJ() except + 
        SimEngineBJ(unsigned int, double, char*, char*, char*, double*) except + 
        char Test()
