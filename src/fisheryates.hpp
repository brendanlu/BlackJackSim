#ifndef FISHERYATES_H
#define FISHERYATES_H
#include "types.hpp"

void FisherYatesShuffle(Card* streamHead, 
                        unsigned int streamSize,
                        unsigned int numShuffle
                        ) {};
/*
FisherYates algorithm to shuffle the cardStream in place. 
numShuffle controls how many shuffled cards we sample in.
*/



// we also need rng (trackable source of randomness), how many shuffles (partial-howmany / full)

#endif