from _cppwrappers cimport StratPackage, InitPackage, SimEngineBJ

"""
Python class for running simulations and benchmarking
"""
cdef class PySimEngineBJ: 
    cdef SimEngineBJ cppSimEngine

    def __init__(self, unsigned int nd, double p): 
        self.cppSimEngine = SimEngineBJ(nd, p)

    def pySetAgentStrat(self, agentIdx, 
    char[:,:] hrd, char[:,:] sft, char[:,:] splt, double[:] cnt):
        self.cppSimEngine.SetAgent(
            agentIdx, 
            &hrd[0][0], 
            &sft[0][0], 
            &splt[0][0], 
            &cnt[0]
        )

    def pyTest(self, unsigned int nIters): 
        self.cppSimEngine.RunSimulation(nIters)
        return 

    # debug for now ------------------------
    def returnCount(self):
        return (self.cppSimEngine.nPlayed,
                self.cppSimEngine.shoeRounds,
                self.cppSimEngine.total)
