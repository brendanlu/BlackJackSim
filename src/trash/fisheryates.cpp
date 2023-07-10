/* 
Fisher Yates shuffling algorithm, with partial shuffle
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
*/

// #include <immintrin.h> // AVX2 intrinsics header file
#include <random>
// #include "fisheryates.hpp"

using unifIntBounds_t = std::uniform_int_distribution<unsigned int>::param_type;

template <typename T, typename randomNumGenerator>
unsigned int FisherYatesShuffle(T* arrHead, unsigned int n, unsigned int nPartial, randomNumGenerator& rng)
/*
FisherYates algorithm to shuffle a contiguous slice of array elements in place. 
nPartial controls partial and incremental shuffle. 
    nPartial corresponds to how many elements we will partially shuffle
    so the first nPartial elements of the array will be new shuffles
    it can take nPartial > n, where it will then default to n 

We take in a reference to our rng, which is external from this, so it preserves its random state. 
The function returns the effective number of items it ends up shuffling. 

When nPartial in {n, n-1}, I think the last iteration of the loop is redundant. 
But in other cases, we need that last iteration for an appropriate partial shuffle. 
*/
{
    unsigned int nEffective = std::min(nPartial, n);
    
    unsigned int j;
    std::uniform_int_distribution<unsigned int> unifInt(0, n-1);
    for (unsigned int i=0; i<=nEffective-1; ++i) 
    {
        unifInt.param(unifIntBounds_t(i, n-1));
        j = unifInt(rng);
        std::swap(*(arrHead+i), *(arrHead+j));
    }

    return nEffective; // return the number of elements shuffled 
};

/* **Explicit instantiation of the FisherYates shuffle for Cards, and a mersenne twister rng */
template unsigned int FisherYatesShuffle<Card, std::mt19937_64>(Card*, unsigned int, unsigned int, std::mt19937_64&);
