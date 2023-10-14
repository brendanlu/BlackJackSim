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
