
#include <random>
#include <iostream>

#include "shoe.hpp"
#include "types.hpp"
#include "fisheryates.hpp"


Shoe::Shoe() {};

Shoe::Shoe(unsigned int NDECKS, double penentration) : 
    mersenneTwister(std::random_device()()), // seed our rng here, when class constructor called
    N_DECKS(N_DECKS),
    N_CARDS(N_DECKS*DECK_SIZE),
    N_UNTIL_CUT(std::min( // if penentation is > 1, we just deal out whole deck stream
        static_cast<unsigned int>(N_CARDS*penentration+0.5),
        N_CARDS
    )),
    nValidShuffled(0),
    nDealt(0),
    nDiscarded(0)
{
    /* *** fill cardStream for N_DECKS */
    unsigned int filledUpTo = 0;
    for (const auto &f: FACE_VALS) {
    for (const auto &s: SUIT_VALS) {
        for (unsigned int i=0; i<N_DECKS; ++i) 
        {
            cardStream[filledUpTo] = {f, s};
            filledUpTo += 1;
        }
    }}

    /* *** fill remaining stack array with BLANK_CARD */ 
    for (; filledUpTo<MAX_DECKS*DECK_SIZE; filledUpTo++) {cardStream[filledUpTo] = BLANK_CARD;}
}

void Shoe::FullShuffle() 
{
    /* Shuffle the whole deck, at the start of a simulation run */
    nValidShuffled = FisherYatesShuffle(&cardStream[0], N_CARDS, N_CARDS, mersenneTwister);
    nDealt = 0; // reset deal status
    nDiscarded = 0;
}

void Shoe::EfficientShuffle(unsigned int from /*=0*/, unsigned int nPartial) 
{
    /*
    We shuffle the shoe a lot in simulations.
    We can save some computation by not fully shuffling the shoe always, 
    but only shuffling as we need. 

    We process the shuffle need to make an appropriate call to the FY shuffle algorithm.
    Also update Shoe status variables appropriately. 

    This method is called by the simulation engine, but also the Shoe itself when 
        it needs to deal fresh cards exceeding nValidShuffled.
    */

    unsigned int nNonBlankRemaining; // for nontrivial cases, compute how many cards until the end of the stream
    unsigned int nSuccessShuffled; // how many did our Fisher Yates shuffle algorithm shuffle fresh for us?

    if (nPartial == 0) { // default is to shuffle up until cut card
        nPartial == N_UNTIL_CUT; 
    }

    if (from == 0) { // just a new partial "reset" shuffle of the shoe
        nValidShuffled = FisherYatesShuffle(&cardStream[0], N_CARDS, nPartial, mersenneTwister); 
        nDealt = 0;
        nDiscarded = 0;
    }
    else { // in this case, we are asking for more shuffled cards, we are still playing the same shoe
        nNonBlankRemaining = N_CARDS - nDealt;
        nSuccessShuffled = FisherYatesShuffle(&cardStream[from], nNonBlankRemaining, nPartial, mersenneTwister);

        nValidShuffled += nSuccessShuffled;
        // WE NEED TO HANDLE THE CASE WHERE THE ACTIVE SHOE ACTUALLY RUNS OUT OF CARDS
            // NOT JUST FRESH CARDS, BUT COMPLETELY OUT OF CARDS
    }
}

void Shoe::Deal(Agent targetAgent) 
{
    /*
    Simulates delaing a card to a player (Agent type).
    Does some simple housekeeping on the Shoe side to make sure deal is ok.
    Then calls the DealHandler method of the target Agent object.

    Returns the success status of this deal. 
    */

    if (nDealt + 1 <= nValidShuffled) { // we have "fresh" shuffled cards to deal
        targetAgent.DealHandler(cardStream[nDealt++]);
    }
    else { // we can just get some more "fresh" shuffled cards in the cardstream
        this->EfficientShuffle(nDealt, 1);
        targetAgent.DealHandler(cardStream[nDealt++]); 
    }
    // SAME PROBLEM - WE NEED TO HANDLE THE CASE WHERE WE DO NOT HAVE ENOUGH REAL CARDS TO DEAL
}

void Shoe::Display()
{
    for (Card card: cardStream) {
        if (card == BLANK_CARD) {break;}
        else {std::cout << card << " ";}
    }
    std::cout << "\n";
}

