
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

void Shoe::FreshShuffleN(unsigned int nPartial) {
    /*
    In practice, the blackjack shoe is very rarely fully dealt. 

    We can save some computation in our simulation loop by only partially shuffling. 
    In the case that we need more "freshly shuffled" cards, we can make use of incremental shuffling. 
    */
    nValidShuffled = FisherYatesShuffle(&cardStream[0], N_CARDS, nPartial, mersenneTwister);
    nDealt = 0;
    nDiscarded = 0; 
}

void Shoe::PushBackShuffle()
{
    // We need more "simulated shuffled" cards to deal out, so we perform a one iteration FY incremental shuffle.
    if (nValidShuffled + 1 <= N_CARDS) {
        nValidShuffled += FisherYatesShuffle(&cardStream[nValidShuffled], N_CARDS - nValidShuffled, 1, mersenneTwister);
    }
    else { 
        // whilst possible, this case should be - in practice, near impossible in a well instantiated simulation game
        //      it corresponds to dealing out the whole shoe midway through a table
        // we basically reshuffle and deal out of the discarded tray, until we clear the table, and then reshuffle the shoe
        needReshuffle = true;
        nDealt = 0; 
        nValidShuffled = FisherYatesShuffle(&cardStream[0], nDiscarded, 1, mersenneTwister);
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
        if (nDealt > N_UNTIL_CUT) {needReshuffle = true;}
    }
    else { // we can just get some more "fresh" shuffled cards in the cardstream
        PushBackShuffle(); 
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

