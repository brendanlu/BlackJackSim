"""
Cython C type method/attribute declarations for the simengine cpp class

https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
"""
cdef extern from "../src/simenginebj.hpp":
    cdef cppclass SimEngineBJ: 
        SimEngineBJ() except + 
        SimEngineBJ(unsigned int, double) except +
        void SetAgent(unsigned int, char*, char*, char*, double*)
        int RunSimulation(unsigned long long)

        int nPlayed
        int shoeRounds
        int total
