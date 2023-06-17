#ifndef TYPES_H
#define TYPES_H 
#include <iostream> // we need this to pass char into std::ostream operator<<

struct card {char face; char suit;};

inline std::ostream& operator<< (std::ostream& os, card const& c) {os<<c.face; os<<c.suit; return os;}
    // inline this so it is only defined once, overload std::cout<< for card_t type 

typedef struct card card_t;

constexpr card_t BLANK_CARD = {'0', '0'};

#endif