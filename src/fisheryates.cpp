/* 
A few algorithms to shuffle the cardstream, based on the Fisher-Yates shuffle.
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

*/


#include <random>
// #include <immintrin.h> // AVX2 intrinsics header file
#include "fisheryates.hpp"

template <typename T, typename randomNumGenerator>
void FisherYatesShuffle(T* begin, T* end, unsigned int iters, randomNumGenerator& rng)
/*
FisherYates algorithm to shuffle the cardStream in place. 
numShuffle controls how many shuffled cards we sample in.
We take in a reference to our rng, which is contained in the simengine class when we call this.
*/
{
    std::uniform_int_distribution<int> distribution(1,100);

    int randomValue = distribution(rng); 
};

/* **Explicit instantiation of the FisherYates shuffle with my Card type and, for now, a mersenne twister rng  */
template void FisherYatesShuffle<Card, std::mt19937_64>
(Card* begin, Card* end, unsigned int iters, std::mt19937_64& mersenneTwister);

