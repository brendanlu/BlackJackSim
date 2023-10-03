"""
Cython C type method/attribute declarations for the simengine cpp class, and 
other relevant C++ classes which we need Cython to understand.  

https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html
"""

cimport cython
from libc.stdlib cimport malloc, free

from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "<iostream>" namespace "std": 
    cdef cppclass ostream: 
        ostream& write(const char*, int) except + 

cdef extern from "../src/strategyinput.hpp":
    cdef cppclass StratPackage:
        char *hrd 
        char *sft
        char *splt
        double *cnt

cdef extern from "../src/simenginebj.hpp":
    cdef unsigned int MAX_N_AGENTS

    cdef cppclass InitPackage:
        unsigned int nDecks 
        double shoePenentration
        bool dealer17
        unsigned int nAgents
        StratPackage *strats

    cdef cppclass SimEngineBJ: 
        SimEngineBJ() except + 
        SimEngineBJ(unsigned int, double) except +
        SimEngineBJ(InitPackage) except +

        void InitNewLogging()

        void SetAgent(unsigned int, char*, char*, char*, double*)
        void SetDealer17(bool)
        void SetLogFile(string)
        void SetLogLevel(int)
        void RunSimulation(unsigned long long)


#@cython.boundscheck(False)
#@cython.wraparound(False)
cdef inline _make_strat_package_arr(strat_list):
    """
    This is not performance critical, so it is prudent to keep the full range of 
    default Python checks and warnings. 

    Parameters
    ----------
    strat_list : List[Tuple[np.ndarray]]
        A list of tuples in the return form from _strat_to_numpy_arrayfmt

    Returns
    -------
    strats_ptr : StratPackage C Pointer
        Points to a newly malloc'ed array of StratPackages sized len(strat_list)

    """

    cdef unsigned int i
    cdef unsigned int j
    for i in range(len(strat_list)): 
        for j in range(len(strat_list[i])): 
            pass

    return
