#ifndef TYPES_H
#define TYPES_H 
#include <iostream> // we need this to overload << for Card_t


// misc constants --------------------------------------------------------------------------------------
constexpr unsigned int BJVAL = 21; 
constexpr unsigned int INIT_DEAL = 2;

// Stuff for our Card representation ---------------------------------------------------------------
constexpr unsigned int VALS[10] = {2,3,4,5,6,7,8,9,10,11};
constexpr int ASCII_2 = 50; // ascii value of 2 
constexpr int ASCII_9 = 57;
constexpr int ASCII_A = 65; 
struct Card {
    char face; 
    char suit;
    // quick, simple way to convert card face chars into ints; their numeric values
    // this operation happens often
    inline unsigned int val() {
        // 2-9 chars 
        if ((int)face <= ASCII_9) {return VALS[(int)face - ASCII_2];}
        // A
        else if (face == 'A') {return VALS[9];}
        // Must be T, J, Q, K
        else {return VALS[8];}
    }
};

inline std::ostream& operator<< (std::ostream& os, const Card& c) {os<<c.face; os<<c.suit; return os;}
    // inline this so it is only defined once, overload std::cout<< for Card type 

inline bool operator== (const Card &c1, const Card& c2) {return ((c1.face==c2.face) && (c1.suit==c2.suit));}
    // similarly inline, so we can compare with blank card

constexpr Card BLANK_CARD = {'0', '0'};

// enumerations ------------------------------------------------------------------------------------
// various error codes
enum class ERR_CODE : int {
    SUCCESS = 0, // generic success code

    // Main simulation loop related ---------------------------------
    NO_AGENT_STRAT = 100,

    // Query agent related --------------------------------------
    INVALID_ACTION = 200
};

// player action codes
enum class ACTION : char {
    HIT = 'H',
    STAND = 'S', 
    DOUBLE = 'D', 
    SPLIT = 'P', 
    SURRENDER = 'R'
};

#endif