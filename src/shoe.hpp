#ifndef SHOE_H
#define SHOE_H 

#include <array>
#include <random>
#include "types.hpp"

/*     
*** This is the only stochastic element of our simulation. 
All the randomness will be encapsulated in this class. 

It also performs the most computationally intensive within the simulation loops:
                simulating shuffling the deck. 

We initialise the cards in the shoe on a stack array, which is declared large enough to
            house 8 decks. 
The Shoe has logic implemented to ignore the rest of the redundant stack space.
*/

class Shoe
{
public:
    Shoe() {};
    Shoe(unsigned int nDecks, double penentration);
    void Shuffle(unsigned int partial = MAX_DECKS*DECK_SIZE+1); // shuffles all by default
    void Display();

    unsigned int NDECKS; 

private:
    std::mt19937_64 mersenneTwister; // we should be able to change this later without breaking too many things

    static constexpr unsigned int MAX_DECKS = 8;
    static constexpr unsigned int DECK_SIZE = 52;
    static constexpr unsigned int N_CARD_FACE_VALS = 13;
    static constexpr unsigned int N_CARD_SUIT_VALS = 4;

    static constexpr std::array<char, N_CARD_FACE_VALS> FACE_VALS = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
    static constexpr std::array<char, N_CARD_SUIT_VALS> SUIT_VALS = {'D', 'S', 'C', 'H'};

    std::array<Card, MAX_DECKS*DECK_SIZE> cardStream; // we initialise this onto the stack, as we manipulate it alot
    unsigned int typicalCardsDealt; // how many cards are dealt before shoe is reshuffled
    unsigned int cardStreamEndIdx; // this may be dodgy, but after this all cardStream stack values are blank
};

#endif
