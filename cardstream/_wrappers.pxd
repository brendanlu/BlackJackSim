"""
Cython C type method/attribute declarations for the simengine cpp class, and 
other relevant C++ classes which we need Cython to understand.  

https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
"""

cimport cython

from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "src/simenginebj.hpp":
    cdef cppclass SimEngineBJ: 
        SimEngineBJ() except + 
        SimEngineBJ(unsigned int, double) except +

        void InitNewLogging()

        void SetAgent(unsigned int, char*, char*, char*, double*)
        void SetDealer17(bool)
        void SetLogFile(string)
        void SetSocketConnection(string, int)
        void SetLogLevel(int)
        void RunSimulation(unsigned long long)
