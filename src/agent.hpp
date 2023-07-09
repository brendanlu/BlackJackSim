#ifndef AGENT_H
#define AGENT_H 

#include "types.hpp"

/*
The Agent is quite a simple class, as most of its game logic comes from the 
    input strategy files. 

Potential downstream users can choose to directly alter this class, to encode some 
    more complex and intricate game variations. 
The logic in the handler methods are the best place to start, and only affect the
    behaviour of the Agent, and are not critical to the correct operation of the simulation. 
*/

constexpr unsigned int MAX_HSIZE = 21; // maximum hand size - 21 soft aces

class Agent 
{
public:
    // cython needs nullary constructor, and this is memeber class of simenginebj
    Agent(); 
    // construct an object by passing in pointers to the strategy template
    Agent(char* hrd, char* sft, char* splt, double* cnt); 

    // logic for recieving one card
    void DealHandler(Card dCard);
    void ShuffleHandler(); 

    long long stackVal; // bankroll 
    bool stratInit; // keeps track of if the pointers below are actually pointing to strats

private:
    double* cntPtr; // pointer to find count values of various cards 
    char* hrdPtr; char* sftPtr; char* spltPtr;  // pointer to strategy input files
    
    double cntVal; // card count as dictated by count input

    // struct to hold relevent information of the hand
    // can adjust relatively easily, and change the relevant methods (& constructor)
    struct HandInfo {
        unsigned int nHolding; // number of cards in hand
        unsigned int handVal; // soft/hard value of the hand, depending on if we have aces
        unsigned int nSoftAces; // number of aces that are (soft) counted as 11 in hand
        
        char lastFace; // for triggering pair flag
        bool holdingPair; // flag if we are holding a pair
    } hInfo;
};

#endif