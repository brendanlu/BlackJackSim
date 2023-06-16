# distutils: language = c++

import numpy as np
cimport simengine

def some_func(int a): 
    return simengine.bounce_back(a)

