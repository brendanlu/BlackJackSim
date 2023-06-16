#ifndef DECK_H
#define DECK_H

typedef struct card {char face; char suit;} card_t;

constexpr unsigned int DECK_SIZE = 52;
constexpr unsigned int N_CARD_FACES = 13;
constexpr unsigned int N_CARD_SUITS = 4;

constexpr std::array<char, N_CARD_FACES> FACES = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
constexpr std::array<char, N_CARD_SUITS> SUITS = {'D', 'S', 'C', 'H'};

#endif