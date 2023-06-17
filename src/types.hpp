#ifndef TYPES_H
#define TYPES_H 

typedef struct card {char face; char suit;} card_t;

enum class Suit : char {
    DIAMONDS = 'D', 
    SPADES = 'S', 
    CLUBS = 'C',
    HEARTS = 'H',
};

enum class Face : int { 
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT, 
    NINE, 
    TEN,
    JACK,
    QUEEN,
    KING,
};


#endif