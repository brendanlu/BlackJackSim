import cardstream

# may change to use numpy to read-in csv, to avoid pandas dependency
import pandas as pd

strat = pd.read_csv(
    r"Strategy Input\BasicNoDeviations-4to8Decks-HitSoft17.csv", header=None
)

"""
Read in the individual strategy tables. Cast into byte chars and C contiguous arrays. 
"""
hard = strat.iloc[0:18, :].copy().reset_index(drop=True)
hard = hard.values.astype(str)
hrd = hard[1:, 1:].astype("S1", order="C")

soft = strat.iloc[18:27, :].copy().reset_index(drop=True)
soft = soft.values.astype(str)
sft = soft[1:, 1:].astype("S1", order="C")

splits = strat.iloc[27:38, :].copy().reset_index(drop=True)
splits = splits.values.astype(str)
splts = splits[1:, 1:].astype("S1", order="C")

# Read in the counting strategy
count = pd.read_csv(r"Strategy Input\HiLoCount.csv", header=None)
cnt = count.iloc[:, 1].values.astype(float, order="C")

tryme = cardstream.PySimEngineBJ(1, 1)
print(tryme.pyTest(1000))
tryme.pySetAgentStrat(0, hrd, sft, splts, cnt)
print("Set agent strat works")
print(hrd)
print(sft)
print(splts)
print(cnt)
print("\nTrying strat template event loop -------------------")
print(tryme.pyTest(100000))
print(tryme.returnCount())
