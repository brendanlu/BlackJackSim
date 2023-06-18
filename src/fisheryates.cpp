/* 
A few algorithms to shuffle the cardstream, based on the Fisher-Yates shuffle.
https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

*/



#include <immintrin.h> // AVX2 intrinsics header file
#include "fisheryates.hpp"


void FisherYatesShuffle(Card* streamHead, 
                        unsigned int streamSize,
                        unsigned int numShuffle
                        ) {};
/*
FisherYates algorithm to shuffle the cardStream in place. 
numShuffle controls how many shuffled cards we sample in.
*/