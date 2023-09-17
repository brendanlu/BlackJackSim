from _cppwrappers cimport StratPackage, InitPackage, SimEngineBJ


cdef class pySimEngineBJ: 
    """
    Cython wrapper class for Python.
    """

    cdef SimEngineBJ cppSimEngineBJ

    
    def __init__(self, unsigned int nd, double p): 
        self.cppSimEngineBJ = SimEngineBJ(nd, p)
    

    def initinprogress(self, nd, p, d17, nA, strats): 
        """
        Parameters
        ----------
        nd : int
            The number of decks in the simulation

        p : float
            Deck penetration

        d17 : bool
            Dealer hits on soft 17

        nA : int
            Number of agents in the simulation

        strats : iterable of length nA
            Each item contains a strat tuple in the output format of 
            _strat_to_numpy_arrayfmt

            These represent the agent configurations from the config files
        """
        return

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
