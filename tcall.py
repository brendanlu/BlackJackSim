import cardstream as cs
from pyinterface import _pyutils

strat_args = _pyutils._strat_to_numpy_arrayfmt(
    strat_relpath=r"pyinterface\teststrat\BasicNoDeviations-4to8Decks-HitSoft17.csv",
    count_relpath=r"pyinterface\teststrat\HiLoCount.csv",
)

print("utils are ok")
tryme = cs.pySimEngineBJ(6, 0.5)
print("Contructed object")
tryme.pySetAgent(0, *strat_args)
tryme.pySetAgent(1, *strat_args)
print("Set agent strat works")
print(strat_args[0])
print(strat_args[1])
print(strat_args[2])
print(strat_args[3])

print("Configuring log file")
tryme._pyInitNewLogging()
tryme.pySetLogLevel(3)
tryme.pySetLogFile("LOG2.csv")

print("\nTrying strat template event loop -------------------")
tryme.pyRunSimulation(5000000)
print("COMPLETE")
