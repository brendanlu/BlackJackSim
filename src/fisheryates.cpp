/* 
A few algorithms to shuffle the cardstream, based on the Fisher-Yates shuffle.
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

*/

// #include <immintrin.h> // AVX2 intrinsics header file
#include <random>
#include <cassert>
#include "fisheryates.hpp"

using unifIntBounds_t = std::uniform_int_distribution<unsigned int>::param_type;

template <typename T, typename randomNumGenerator>
void FisherYatesShuffle(T* begin, T* end, unsigned int nPartial, randomNumGenerator& rng)
/*
FisherYates algorithm to shuffle a contiguous slice of array elements in place. 
iters controls partial and incremental shuffle. 
    iter corresponds to how many elements we will partially shuffle
We take in a reference to our rng, which is external from this, so it preserves its random state. 
*/
{
    int n = end - begin + 1; 
    // assert(n > 0);
    unsigned int nEffective = std::min(nPartial, static_cast<unsigned int>(n));
    
    unsigned int j;
    std::uniform_int_distribution<unsigned int> unifInt(0, n);
    for (unsigned int i=0; i<=nEffective-1; ++i) 
    {
        unifInt.param(unifIntBounds_t(i, n-1));
        j = unifInt(rng);
        std::swap(*(begin+i), *(begin+j));
    }
};

/* **Explicit instantiation of the FisherYates shuffle for Cards, and a mersenne twister rng */
template void FisherYatesShuffle<Card, std::mt19937_64>
(Card* begin, Card* end, unsigned int iters, std::mt19937_64& mersenneTwister);
