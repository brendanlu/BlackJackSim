from simengine cimport SimEngineBJ

"""
Wrapper class that can be used in Python which contains an instsance of the cpp 
class object as a member. 
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

    def returnCount(self):
        return (self.cppSimEngine.nPlayed,
                self.cppSimEngine.shoeRounds,
                self.cppSimEngine.total)


"""
Low priority: Write some Python errors to hand the ERR_CODES and give 
information back out here.
"""