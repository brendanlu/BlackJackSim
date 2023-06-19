# distutils: language = c++

import numpy as np
from simengine cimport SimEngineBJ

cdef class PySimEngineBJ: 
    cdef SimEngineBJ cppSimEngine

    def __init__(self, unsigned int nd, double p, 
    char[:,:] hrd, 
    char[:,:] sft, 
    char[:,:] splt, 
    double[:] cnt): 
        self.cppSimEngine = SimEngineBJ(nd, p, &hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0])

    def pyTest(self): 
        return <bytes>(self.cppSimEngine.Test())

