
#include <random>
#include <cassert>
#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"
#include "fisheryates.hpp"


// temp ----------------------------------------------
#include <unordered_map>
#include <algorithm>
#include <array>
//-----------------------------------------------------
Shoe::Shoe(unsigned int nDecks) : 
    NDECKS(nDecks),
    mersenneTwister(std::random_device()()), // seed our rng here
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
    // for (auto thing: cardStream) {cout<< thing;}
}

void Shoe::Shuffle() 
{
    FisherYatesShuffle(&cardStream[0], &cardStream[cardStreamEndIdx], 
                        NDECKS*DECK_SIZE, // full shuffle
                        mersenneTwister);
}

void Shoe::Display()
{
    for (Card card: cardStream) {
        if (card == BLANK_CARD) {break;}
        else {cout << card << " ";}
    }
    cout << "\n";
}