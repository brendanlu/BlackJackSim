# distutils: language = c++

import numpy as np
cimport SimEngine

def some_func(int a): 
    return SimEngine.bounce_back(a)

