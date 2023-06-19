# distutils: language = c++

import numpy as np
from simengine cimport SimEngineBJ

cdef class PySimEngineBJ: 
    cdef SimEngineBJ cppSimEngine

    def __init__(self, unsigned int nd, double p): 
        # we read in memviews of our numpy arrays - THESE MUST BE C CONTIGUOUS BYTE ARRAYS
        # we then pass in the pointer to the first element to our cpp level class constructor
        self.cppSimEngine = SimEngineBJ(nd, p)

        # self.cppSimEngine = SimEngineBJ(nd, p, &hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0])

    def pySetAgentStrat(self, char[:,:] hrd, char[:,:] sft, char[:,:] splt, double[:] cnt):
        self.cppSimEngine.SetAgentStrat(&hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0])

    def pyTest(self): 
        return (self.cppSimEngine.Test())

