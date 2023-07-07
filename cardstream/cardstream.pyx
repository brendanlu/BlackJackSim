# distutils: language = c++

import numpy as np
from simengine cimport SimEngineBJ

# Wrapper class that can be used in Python
# contains the c object as a member, which has its methods called in the Python wrapper methods
cdef class PySimEngineBJ: 
    cdef SimEngineBJ cppSimEngine

    def __init__(self, unsigned int nd, double p): 
        # we read in memviews of our numpy arrays - THESE MUST BE C CONTIGUOUS BYTE ARRAYS
        self.cppSimEngine = SimEngineBJ(nd, p)

    def pySetAgentStrat(self, char[:,:] hrd, char[:,:] sft, char[:,:] splt, double[:] cnt):
        # we then pass in the pointer to the first element to our cpp level BJ Agent
        self.cppSimEngine.SetAgentStrat(&hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0])

    def pyTest(self, unsigned int nIters): 
        return <int>self.cppSimEngine.RunSimulation(nIters)
