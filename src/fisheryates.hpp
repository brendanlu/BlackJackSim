#ifndef FISHERYATES_H
#define FISHERYATES_H

#include <random>
#include "types.hpp"

using unifIntBounds_t = std::uniform_int_distribution<unsigned int>::param_type;


/*
FisherYates algorithm to shuffle a contiguous slice of array elements in place. 
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

This is the 'forward' version, whereby we place our newly shuffled items at the
front of the array, sampling from the end. 

For now, the function returns successful number of items shuffled. 

PARAMS:
    :arrHead: pointer to the start of the array
    :n: the size of the array following arrHead, which we can sample from
    :nPartial: how many newly shuffled elements to be sampled to the front
    :rng: reference to a random number generator adhering to C++11 <random> API

NOTE: 
    When nPartial in {n, n-1}, the last iteration of the loop should be 
    redundant. But in other cases, the last iteration should be needed for an 
    appropriate partial shuffle. 
*/
template <typename T, typename randomNumGenerator>
unsigned int FYShuffle(
    T* arrHead, 
    unsigned int n, 
    unsigned int nPartial, 
    randomNumGenerator &rng) 
{
    // first take the minimum of the partial n request and the max n
    // this may change
    unsigned int nEffective = std::min(nPartial, n); 
    
    unsigned int j;
    std::uniform_int_distribution<unsigned int> unifInt(0, n-1);
    for (unsigned int i=0; i<=nEffective-1; ++i) 
    {
        unifInt.param(unifIntBounds_t(i, n-1));
        j = unifInt(rng);
        std::swap(*(arrHead+i), *(arrHead+j));
    }

    return nEffective; 
};

#endif
