#ifndef DECK_H
#define DECK_H
#include <array>

typedef struct card {char face; char suit;} Card;

constexpr unsigned int DECK_SIZE = 52;
constexpr unsigned int N_CARD_FACE_VALS = 13;
constexpr unsigned int N_CARD_SUIT_VALS = 4;
constexpr unsigned int MAX_DECKS = 8; // constexpr as we stack allocate this memory
constexpr std::array<char, N_CARD_FACE_VALS> FACE_VALS = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
constexpr std::array<char, N_CARD_SUIT_VALS> SUIT_VALS = {'D', 'S', 'C', 'H'};

// stack allocate array to hold the cards, we manipulate this and index it very often
// constexpr std::array<card, MAX_DECKS*DECK_SIZE> cardStream;

// fill values in the cardstream
unsigned int filled_idx = 0;

/*
for (const auto &face: FACE_VALS) {
for (const auto &suit: SUIT_VALS) {
for (int i=0; i<MAX_DECKS; i++) {

    }}}
*/



#endif