#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include <array>
#include "types.hpp"

// char bounce_back(int a);

class SimEngine
{
public:
    SimEngine(unsigned int testin);
    
    unsigned int test; 

private:
    static const unsigned int DECK_SIZE = 52;
    static const unsigned int N_CARD_FACES = 13;
    static const unsigned int N_CARD_SUITS = 4;
    static const unsigned int MAX_DECKS = 8;
    // static const std::array<char, N_CARD_FACES> FACES = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
    // static const std::array<char, N_CARD_SUITS> SUITS = {'D', 'S', 'C', 'H'};
};


#endif
