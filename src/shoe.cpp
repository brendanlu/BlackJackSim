
#include <random>
#include <cassert>
#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"
#include "fisheryates.hpp"

Shoe::Shoe(unsigned int nDecks) : 
    NDECKS(nDecks),
    mersenneTwister(std::random_device()()), // seed our rng here, when class constructor called
    cardStreamEndIdx(NDECKS*DECK_SIZE - 1)
{
    /* *** fill cardStream for NDECKS */
    unsigned int filledUpTo = 0;
    for (const auto &f: FACE_VALS) {
    for (const auto &s: SUIT_VALS) {
        for (int i=0; i<NDECKS; i++) 
        {
            cardStream[filledUpTo] = {f, s};
            filledUpTo += 1;
        }
    }}
    
    assert((filledUpTo - 1) == cardStreamEndIdx);

    /* *** fill remaining stack array with BLANK_CARD */ 
    for (; filledUpTo<MAX_DECKS*DECK_SIZE; filledUpTo++) {cardStream[filledUpTo] = BLANK_CARD;}
}

void Shoe::Shuffle(unsigned int partial /* = MAX_DECKS*DECK_SIZE  */) 
{
    /* 
    ***By default we will shuffle the whole deck.
    If one passes in a smaller int than the size of the shoe, it will partial shuffle.
     */
    FisherYatesShuffle(&cardStream[0], &cardStream[cardStreamEndIdx], partial, mersenneTwister);
}

void Shoe::Display()
{
    for (Card card: cardStream) {
        if (card == BLANK_CARD) {break;}
        else {cout << card << " ";}
    }
    cout << "\n";
}