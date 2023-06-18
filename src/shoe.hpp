#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include <array>
#include <random>
#include "types.hpp"

// char bounce_back(int a);

class Shoe
{
public:
    Shoe(unsigned int nDecks);
    void Shuffle();
    void Display();

    unsigned int NDECKS; 

private:
    std::mt19937_64 mersenneTwister; // we should be able to change this

    static constexpr unsigned int MAX_DECKS = 8;
    static constexpr unsigned int DECK_SIZE = 52;
    static constexpr unsigned int N_CARD_FACE_VALS = 13;
    static constexpr unsigned int N_CARD_SUIT_VALS = 4;

    static constexpr std::array<char, N_CARD_FACE_VALS> FACE_VALS = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
    static constexpr std::array<char, N_CARD_SUIT_VALS> SUIT_VALS = {'D', 'S', 'C', 'H'};

    std::array<Card, MAX_DECKS*DECK_SIZE> cardStream;
    unsigned int cardStreamEndIdx; // this may be dodgy, but after this all cardStream values are blank
};


#endif
