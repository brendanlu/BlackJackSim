#ifndef TYPES_H
#define TYPES_H 
#include <iostream> // we need this to pass char into std::ostream operator<<

struct Card {char face; char suit;};

inline std::ostream& operator<< (std::ostream& os, Card const& c) {os<<c.face; os<<c.suit; return os;}
    // inline this so it is only defined once, overload std::cout<< for Card type 

inline bool operator== (Card c1, const Card c2) {return ((c1.face==c2.face) && (c1.suit==c2.suit));}
    // similarly inline, so we can compare with blank card

constexpr Card BLANK_CARD = {'0', '0'};

// struct to hold relevent simple information of the hand
struct HandInfo {
    unsigned int hrdVal; // hard value of the hand 
    unsigned int sftVal; // soft value of the hand
};

constexpr unsigned int MAX_HSIZE = 21; // maximum hand size - 21 soft aces

#endif
