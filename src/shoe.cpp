
#include <random>
#include <iostream>

#include "shoe.hpp"
#include "types.hpp"
#include "fisheryates.hpp"

/*
Explicit instation of Fisher Yates shuffling algorithm for the custom Card
type and a 64 bit Mersenne Twister. 
*/
template unsigned int FYShuffle<Card, std::mt19937_64>(
    Card*, 
    unsigned int, 
    unsigned int, 
    std::mt19937_64&);


/*
Cython needs a nullary constructor, this will never be called from Python.
*/
template <typename RNG>
Shoe<RNG>::Shoe() {};


/*
We seed our random number generator in the constructor.
*/
template <typename RNG>
Shoe<RNG>::Shoe(unsigned int NDECKS, double penentration) : 
rng(std::random_device()()), 
N_DECKS(NDECKS),
N_CARDS(N_DECKS*DECK_SIZE),
N_UNTIL_CUT(std::min(std::round(N_CARDS*penentration), N_CARDS)),
nShuffled(0),
nDealt(0),
nDiscarded(0),
needReshuffle(false)
{
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
