from PySimEngineBJ import PySimEngineBJ
from cardstream import _pyutils

strat_args = _pyutils._strat_to_numpy_arrayfmt(
    strat_relpath=r"cardstream\teststrat\BasicNoDeviations-4to8Decks-HitSoft17.csv",
    count_relpath=r"cardstream\teststrat\HiLoCount.csv",
)

tryme = PySimEngineBJ(6, 0.5)
print(tryme.pyTest(1000))
tryme.pySetAgentStrat(0, *strat_args)
print("Set agent strat works")
print(strat_args[0])
print(strat_args[1])
print(strat_args[2])
print(strat_args[3])
print("\nTrying strat template event loop -------------------")
print(tryme.pyTest(100000))
print(tryme.returnCount())
