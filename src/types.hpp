#ifndef TYPES_H
#define TYPES_H 

#include <array>
#include <iostream>

/*
misc. constants
*/
constexpr unsigned int BJVAL = 21; 
constexpr unsigned int INIT_DEAL = 2;
constexpr int ASCII_2 = 50; // ascii value of 2 
constexpr int ASCII_9 = 57;
constexpr int ASCII_A = 65; 

/*
A simple struct to represent a card with minimal memory overhead and efficient
value retrieval, whilst maintaining full face and suit information. 
*/
constexpr unsigned int N_CARD_FACE_VALS = 13;
constexpr unsigned int N_DISTINCT_NUMERIC = 10;
constexpr unsigned int N_CARD_SUIT_VALS = 4;

constexpr std::array<char, N_CARD_FACE_VALS> 
FACE_VALS = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};

constexpr std::array<unsigned int, N_DISTINCT_NUMERIC>
NUMERIC_VALS = {2,3,4,5,6,7,8,9,10,11};

constexpr std::array<char, N_CARD_SUIT_VALS> 
SUIT_VALS = {'D','S','C','H'};

struct Card {
    char face; 
    char suit;

    /*
    Quick way to get the numeric value of the card
    */
    inline unsigned int val() const 
    {
        // 2-9
        if ((int)face <= ASCII_9) {
            return NUMERIC_VALS[(int)face - ASCII_2];
        }
        // A
        else if (face == 'A') {
            return NUMERIC_VALS[9];
        }
        // T, J, Q, K
        else {
            return NUMERIC_VALS[8];
        }
    }

    /*
    Overload bool() operator to check if the Card is empty. 
    We reserve the '0' char for this purpose.
    */
    inline operator bool() const
    {
        return face != '0';
    }
};

constexpr Card BLANK_CARD = {'0', '0'};

// overload <<, inline the definition so it is not redefined each #include
inline std::ostream& operator<< (std::ostream& os, const Card& c) 
{
    os << c.face; 
    os << c.suit; 
    return os;
}

// overload ==, inline the definition so it is not redefined each #include
inline bool operator== (const Card &c1, const Card& c2) 
{
    return ((c1.face==c2.face) && (c1.suit==c2.suit));
}

/*
Various error codes
*/
enum class ERR_CODE : int 
{
    // generic success code
    SUCCESS = 0, 

    // codes prefixed with 1
    // main sim loop related
    NO_AGENT_STRAT = 101,
};

/*
The valid actions the simulation engine can process
*/
enum class ACTION : char 
{
    HIT = 'H',
    STAND = 'S', 
};

#endif