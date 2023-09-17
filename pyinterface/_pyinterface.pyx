from _cppwrappers cimport StratPackage, InitPackage, SimEngineBJ


cdef class pySimEngineBJ: 
    """
    Cython wrapper class for Python.
    """

    cdef SimEngineBJ cppSimEngineBJ

    def __init__(self, unsigned int nd, double p): 
        self.cppSimEngineBJ = SimEngineBJ(nd, p)

    def pySetAgent(
        self, 
        agentIdx, 
        char[:,:] hrd, 
        char[:,:] sft, 
        char[:,:] splt, 
        double[:] cnt
    ):
        self.cppSimEngineBJ.SetAgent(
            agentIdx, &hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0]
        )

    def pySetLogFile(self, filename):
        self.cppSimEngineBJ.SetLogFile(filename.encode('UTF-8'))
        return

    def pySetLogLevel(self, int ll) : 
        self.cppSimEngineBJ.SetLogLevel(ll)
        return

    def pyRunSimulation(self, unsigned int nIters): 
        self.cppSimEngineBJ.RunSimulation(nIters)
        return 
