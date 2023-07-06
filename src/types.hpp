#ifndef TYPES_H
#define TYPES_H 
#include <iostream> // we need this to pass char into std::ostream operator<<


constexpr unsigned int VALS[10] = {2,3,4,5,6,7,8,9,10,11};
constexpr int ASCII_2 = 50; // ascii value of 2 
constexpr int ASCII_9 = 57;
constexpr int ASCII_A = 65; 
struct Card {
    char face; 
    char suit;
    // we need a quick way to convert card face chars into ints; their numeric values
    // this operation happens very often, so we have tried to design an efficient implementation here
    inline unsigned int val() {
        if ((int)face <= ASCII_9) { // 2-9 chars 
            return VALS[(int)face - ASCII_2]; 
        }
        else if ((int)face == ASCII_A) { // A
            return VALS[9];
        }
        else { // Must be T, J, Q, K
            return VALS[8];
        }
    }
};

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