#ifndef SHOE_H
#define SHOE_H 

#include <array>
#include <random>
#include "types.hpp"

/*
Templated Shoe class for different types of psuedo random number generators
that adhere to the C++11 <random> API. 

We initialise the cards in the shoe on a stack array, which is declared large 
enough to house 8 decks. 

The Shoe has logic implemented to ignore the rest of the redundant stack space.
*/
template <typename RNG>
class Shoe
{
public:
    Shoe();
    Shoe(unsigned int NDECKS, double penentration);

    unsigned int N_DECKS; 
    // number of non blank cards in stream
    unsigned int N_CARDS; 
    // how many cards are dealt before draw an (imaginary) cut card
    unsigned int N_UNTIL_CUT; 

    // valid shuffled cards
    unsigned int nShuffled; 
    // keep track of index used in the cardstream
    unsigned int nDealt; 
    // these are cards dealt, and also off the table in the discard tray
    unsigned int nDiscarded; 

    // flag that cut card has been drawn
    bool needReshuffle; 

    void FreshShuffleN(unsigned int nPartial);
    void PushBackShuffle();

    Card Deal(); 

    void Clear(); 
    
    void Display();
    
private:
    static constexpr unsigned int MAX_DECKS = 8;
    static constexpr unsigned int DECK_SIZE = 52;
    static constexpr unsigned int N_CARD_FACE_VALS = 13;
    static constexpr unsigned int N_CARD_SUIT_VALS = 4;

    static constexpr std::array<char, N_CARD_FACE_VALS> 
    FACE_VALS = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};

    static constexpr std::array<char, N_CARD_SUIT_VALS> 
    SUIT_VALS = {'D', 'S', 'C', 'H'};

    RNG rng;

    std::array<Card, MAX_DECKS*DECK_SIZE> cardStream; 
};

#endif
