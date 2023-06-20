#ifndef FISHERYATES_H
#define FISHERYATES_H

#include "types.hpp"

template <typename T, typename randomNumGenerator>
void FisherYatesShuffle(T* arrHead, unsigned int n, unsigned int nPartial, randomNumGenerator& rng);

#endif
