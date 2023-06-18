/* 
A few algorithms to shuffle the cardstream, based on the Fisher-Yates shuffle.
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

*/


#include <random>
#include <cassert>
// #include <immintrin.h> // AVX2 intrinsics header file

#include "fisheryates.hpp"

//temp for debugging --------------------------------
#include <iostream>
//----------------------------


template <typename T, typename randomNumGenerator>
void FisherYatesShuffle(T* begin, T* end, unsigned int iters, randomNumGenerator& rng)
/*
FisherYates algorithm to shuffle a contiguous slice of array elements in place. 
iters controls partial and incremental shuffle; iters = len(end - begin) is a full shuffle of the slice
We take in a reference to our rng, which is external from this, so it preserves its random state. 
*/
{
    int width = end - begin; 
    assert(width > 0);
    
    // uniform integer sampling transform
    std::uniform_int_distribution<unsigned int> unifInt(0, width);

    assert(begin + width == end);



    std::uniform_int_distribution<int> distribution(1,100);

    int randomValue = distribution(rng); 

    std::cout << "RANDOM VALUE:    " << randomValue << "\n"; 


    std::cout << "\nWIDTH = " << width << "\n";



};

/* **Explicit instantiation of the FisherYates shuffle for Card struct and, for now, a mersenne twister rng */
template void FisherYatesShuffle<Card, std::mt19937_64>
(Card* begin, Card* end, unsigned int iters, std::mt19937_64& mersenneTwister);

