
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

# Cython C type method/attribute declarations for the simengine cpp class
cdef extern from "../src/simenginebj.hpp":
    cdef cppclass SimEngineBJ: 
        SimEngineBJ() except + 
        SimEngineBJ(unsigned int, double) except +
        # SimEngineBJ(unsigned int, double, char*, char*, char*, double*) except + 
        void SetAgent(unsigned int, char*, char*, char*, double*)
        int RunSimulation(unsigned long long)

        long long nPlayed
