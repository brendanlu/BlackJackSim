#ifndef DECK_H
#define DECK_H
#include <array>

typedef struct card {char face; char suit;} card_t;

constexpr unsigned int DECK_SIZE = 52;
constexpr unsigned int N_CARD_FACES = 13;
constexpr unsigned int N_CARD_SUITS = 4;
constexpr unsigned int MAX_DECKS = 8; // constexpr as we stack allocate this memory
constexpr std::array<char, N_CARD_FACES> FACES = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
constexpr std::array<char, N_CARD_SUITS> SUITS = {'D', 'S', 'C', 'H'};

// stack allocate array to hold the cards, we manipulate this and index it very often
// constexpr std::array<card, MAX_DECKS*DECK_SIZE> CARDSTREAM;

// fill values in the cardstream
unsigned int filled_idx = 0;

/*
for (const auto &face: FACES) {
for (const auto &suit: SUITS) {
for (int i=0; i<MAX_DECKS; i++) {

    }}}
*/



#endif