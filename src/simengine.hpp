#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include <array>
#include "types.hpp"

// char bounce_back(int a);

class SimEngine
{
public:
    SimEngine(unsigned int deckIn);

    void shuffle();





    unsigned int NDECKS; 

private:
    static constexpr unsigned int MAX_DECKS = 8;
    static constexpr unsigned int DECK_SIZE = 52;
    static constexpr unsigned int N_CARD_FACE_VALS = 13;
    static constexpr unsigned int N_CARD_SUIT_VALS = 4;

    static constexpr std::array<char, N_CARD_FACE_VALS> FACE_VALS = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
    static constexpr std::array<char, N_CARD_SUIT_VALS> SUIT_VALS = {'D', 'S', 'C', 'H'};

    std::array<card_t, MAX_DECKS*DECK_SIZE> cardStream;
};


#endif
