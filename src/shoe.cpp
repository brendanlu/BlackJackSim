
#include <random>
#include <iostream>

#include "shoe.hpp"
#include "types.hpp"
#include "fisheryates.hpp"
#include "simenginebj.hpp"

// Cython needs a nullary constructor
Shoe::Shoe() {};

Shoe::Shoe(unsigned int NDECKS, double penentration) : 
                                 mersenneTwister(std::random_device()()), // seed our rng here, when class constructor called
                                                         N_DECKS(NDECKS),
                                              N_CARDS(N_DECKS*DECK_SIZE),
N_UNTIL_CUT(std::min((unsigned int)(N_CARDS*penentration+0.5), N_CARDS)), // could use std::round()
                                                       nValidShuffled(0),
                                                               nDealt(0),
                                                           nDiscarded(0),
                                                    needReshuffle(false)
{
    /* *** fill cardStream for N_DECKS */
    unsigned int filledUpTo = 0;

    for (const char &f: FACE_VALS) {
    for (const char &s: SUIT_VALS) {
        for (unsigned int i=0; i<N_DECKS; ++i) {cardStream[filledUpTo++] = {f, s};}
    }}

    /* *** fill remaining stack array with BLANK_CARD */ 
    for (; filledUpTo<MAX_DECKS*DECK_SIZE; ++filledUpTo) {cardStream[filledUpTo] = BLANK_CARD;}
}

void Shoe::EfficientShuffle(unsigned int from, unsigned int nPartial) 
{
    /*
    We shuffle the shoe a lot in simulations.
    We can save some computation by not fully shuffling the shoe always, 
    but only shuffling as we need. 

    We process the shuffle request to make an appropriate call to the FY shuffle algorithm.
        Also update Shoe internal variables appropriately. 

    This method is called by the simulation engine, but also the Shoe itself when 
        it needs to deal fresh cards exceeding nValidShuffled.
    */

    unsigned int nSuccessShuffled; // how many new shuffled cards successfuly generated

    if (nPartial == 0) { // default is to shuffle up until cut card
        nPartial == N_UNTIL_CUT; 
    }

    if (from == 0) { // just a new partial "reset" shuffle of the shoe
        nDealt = 0;
        nValidShuffled = FisherYatesShuffle(&cardStream[0], N_CARDS, nPartial, mersenneTwister); 
        nDiscarded = 0;
    }
    else { // in this case, we are asking for more shuffled cards; we are still playing the same shoe
        nSuccessShuffled = FisherYatesShuffle(&cardStream[from], N_CARDS - from, nPartial, mersenneTwister);
        nValidShuffled += nSuccessShuffled;

        // this is the rare case where our shoe actually runs out
        // in this case, we reshuffle the discarded cards already dealt out
        // we then trigger a flag to reshuffle everything the next time we collect all the cards
        if (nSuccessShuffled < nPartial) {
            nDealt = 0;
            nValidShuffled = FisherYatesShuffle(&cardStream[0], nDiscarded, nPartial - nSuccessShuffled, mersenneTwister);
        }
    }
}

template<typename targetType> void Shoe::Deal(targetType &target) 
{
    /*
    Simulates delaing a card to a player (Agent type).
    Does some simple housekeeping on the Shoe side to make sure deal is ok.
    Then calls the DealHandler method of the target Agent object.

    Returns the success status of this deal. 
    */

    if (nDealt + 1 <= nValidShuffled) { // we have "fresh" shuffled cards to deal
        target.DealHandler(cardStream[nDealt++]);
    }
    else { // we can just get some more "fresh" shuffled cards in the cardstream
        needReshuffle = true;
        EfficientShuffle(nDealt, 1);
        Deal(target);
    }
}

// we deal to Agent classes, and the struct in the simengine representing a dealer
template void Shoe::Deal<Agent>(Agent&); 
template void Shoe::Deal<SimEngineBJ::Dealer>(SimEngineBJ::Dealer&); 

void Shoe::Clear() {
    // simulates clearing the table of cards
    nDiscarded = nDealt; // bring count of discarded up to date with dealt
}

void Shoe::Display()
{
    for (Card card: cardStream) {
        if (card == BLANK_CARD) {break;}
        else {std::cout << card << " ";}
    }
    std::cout << "\n";
}

