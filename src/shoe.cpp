#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>

#include "fisheryates.hpp"
#include "shoe.hpp"
#include "types.hpp"


/*
All member classes of the simulation engine have explicit nullary constructors 
provided. This is a design decision to minimise issues with the Cython wrappers.
*/
template <typename RNG>
Shoe<RNG>::Shoe() {};


/*
We seed our random number generator in the constructor.
*/
template <typename RNG>
Shoe<RNG>::Shoe(unsigned int nd, double p) : 
    rng(std::random_device()()), 
    N_DECKS(nd),
    N_CARDS(N_DECKS*DECK_SIZE),
    N_UNTIL_CUT(std::min((unsigned int)std::round(N_CARDS*p), N_CARDS)),
    nShuffled(0),
    nDealt(0),
    nDiscarded(0),
    needReshuffle(false)
{
    // prevent memory unsafe behaviour resulting from a dodgy input
    if (N_DECKS > MAX_DECKS) {
        N_DECKS = MAX_DECKS; 
    }
    
    // fill cardStream for N_DECKS 
    unsigned int filledUpTo = 0;
    for (const char &f: FACE_VALS) {
        for (const char &s: SUIT_VALS) {
            for (unsigned int i=0; i<N_DECKS; ++i) {
                cardStream[filledUpTo++] = {f, s};
            }
        }
    }

    // fill remaining stack array with BLANK_CARD  
    for (; filledUpTo<MAX_DECKS*DECK_SIZE; ++filledUpTo) {
        cardStream[filledUpTo] = BLANK_CARD;
    }
}


/*
This will conduct a 'fresh' shuffle of the shoe.

In practice, we never use the whole entire shoe, so the method lets one pass in 
the number of cards they want to shuffle, to save computation time. 

We can ask for further simulated shuffled cards using the 'push-back' shuffle
method of this class. 
*/
template <typename RNG>
void Shoe<RNG>::FreshShuffleN(unsigned int nPartial) 
{
    nShuffled = FYShuffle(&cardStream[0], N_CARDS, nPartial, rng);
    nDealt = 0;
    nDiscarded = 0; 
    needReshuffle = false; 
}


/*
We append one more new randomly sampled cards to the current nDealt of the 
cardstream. 
*/
template <typename RNG>
void Shoe<RNG>::PushBackShuffle()
{
    // shuffle one more card to the current nDealt position
    if (nShuffled + 1 <= N_CARDS) {
        FYShuffle(&cardStream[nShuffled], N_CARDS - nShuffled, 1, rng);
        nShuffled += 1;
    }
    // accomodate for the case where the whole cardstream has already been used
    // the simulation can still proceed by getting 'fresh' shuffled cards
    // from those which are not in use
    //
    // in a well instatiated simulation - this should never happen 
    else { 
        needReshuffle = true;
        nDealt = 0; 
        nShuffled = FYShuffle(&cardStream[0], nDiscarded, 1, rng);
    }
}


/*
Yields the next card in the cardstream, and does some housekeeping. 

Check to make sure that this is a new 'random' card, resulting from random
sampling into its array position. 

If not, get a new fresh card somehow, and recursively call itself again. 
*/
template <typename RNG>
Card Shoe<RNG>::Deal() 
{
    if (nDealt + 1 <= nShuffled) { 
        if (nDealt + 1 > N_UNTIL_CUT) {
            needReshuffle = true;
        }
        return cardStream[nDealt++];   
    }
    else { 
        PushBackShuffle(); 
        return Deal();
    }
}


/*

*/
template <typename RNG>
void Shoe<RNG>::Clear() {
    // bring count of discarded to that of what has exited the shoe
    nDiscarded = nDealt; 
}


/*

*/
template <typename RNG>
void Shoe<RNG>::Display()
{
    for (Card card: cardStream) {
        if (!card) {break;}
        else {std::cout << card << " ";}
    }
    std::cout << "\n";
}


/*
Explicit instation of Fisher Yates shuffling algorithm for the custom Card type 
and a 64 bit Mersenne Twister. 
*/
template unsigned int FYShuffle<Card, std::mt19937_64>(
    Card* arrHead, 
    unsigned int n, 
    unsigned int nPartial, 
    std::mt19937_64 &rng);

template class Shoe<std::mt19937_64>; 