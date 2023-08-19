"""
Cython C type method/attribute declarations for the simengine cpp class, and 
other relevant C++ classes which we need Cython to understand.  

https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
"""


cdef extern from "<iostream>" namespace "std": 
    cdef cppclass ostream: 
        ostream& write(const char*, int) except + 


cdef extern from "../src/simenginebj.hpp":
    cdef cppclass SimEngineBJ: 
        SimEngineBJ() except + 
        SimEngineBJ(unsigned int, double) except +

        void SetAgent(unsigned int, char*, char*, char*, double*)
        void RunSimulation(unsigned long long)


        # debug stuff for now --------------------------------
        int nPlayed
        int shoeRounds
        int total
