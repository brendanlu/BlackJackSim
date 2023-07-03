
#include <random>
#include <iostream>

#include "shoe.hpp"
#include "types.hpp"
#include "fisheryates.hpp"


Shoe::Shoe() {};

Shoe::Shoe(unsigned int nDecks, double penentration) : 
    mersenneTwister(std::random_device()()), // seed our rng here, when class constructor called
    NDECKS(nDecks),
    nNonBlank(NDECKS*DECK_SIZE),
    nTilCut(std::min(
        static_cast<unsigned int>(NDECKS*DECK_SIZE*penentration+0.5),
        NDECKS*DECK_SIZE
    )), // if penentation is > 1, we just deal out whole deck stream
    nValidShuffled(0), 
    nTilCutt(0)
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
    
    // assert(filledUpTo == nNonBlank);

    /* *** fill remaining stack array with BLANK_CARD */ 
    for (; filledUpTo<MAX_DECKS*DECK_SIZE; filledUpTo++) {cardStream[filledUpTo] = BLANK_CARD;}
}

void Shoe::Shuffle(unsigned int partial /* = MAX_DECKS*DECK_SIZE+1 */) 
{
    /* 
    ***By default we will shuffle the whole deck.
    If one passes in a smaller int than the size of the shoe, it will partial shuffle.
     */
    nValidShuffled = FisherYatesShuffle(&cardStream[0], nNonBlank, partial, mersenneTwister);
    nTilCutt = 0; // reset deal status
}

bool Shoe::Deal(Agent targetAgent) 
{
    /*
    Simulates delaing a card to a player (Agent_t).

    Returns the success status of this deal. 
    */
    if (nTilCutt <= nValidShuffled) {
        targetAgent.dealHandler(cardStream[nTilCutt++]);
        return true;
    }
    else {return false;}
}

void Shoe::Display()
{
    for (Card card: cardStream) {
        if (card == BLANK_CARD) {break;}
        else {std::cout << card << " ";}
    }
    std::cout << "\n";
}

