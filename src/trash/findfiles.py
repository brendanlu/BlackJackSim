"""
used in root, not src folder.
"""

import os

srcs = []
for file in os.listdir("src/"): 
    if file.endswith(".cpp"): 
        srcs.append("src/" + file)

srcs.append("cardstream/cardstream.pyx")
print(srcs)

