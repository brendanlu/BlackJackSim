import cardstream as cs
from pyinterface import _pyutils

strat_args = _pyutils._strat_to_numpy_arrayfmt(
    strat_relpath=r"pyinterface\teststrat\BasicNoDeviations-4to8Decks-HitSoft17.csv",
    count_relpath=r"pyinterface\teststrat\HiLoCount.csv",
)

print("Utils are working")

print("Testing constructor")
testSim = cs.Simulator(
    6, 0.5, True, 3, [strat_args for i in range(3)]
)

print("Configuring log file")
testSim._py_set_log_level(3)
testSim._py_set_log_file("LOG2.csv")

print("Trying strat template event loop".center(20, '-'))
testSim.run(5000000)
print("COMPLETE")
