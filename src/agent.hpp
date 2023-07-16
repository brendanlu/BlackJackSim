#ifndef AGENT_H
#define AGENT_H 

#include "dealer.hpp"
#include "types.hpp"

/*
The Agent is quite a simple class, as most of its game logic comes from the 
    input strategy files. 

Potential downstream users can choose to directly alter this class, to encode some 
    more complex and intricate game variations. 
The logic in the handler methods are the best place to start, and only affect the
    behaviour of the Agent, and are not critical to the correct operation of the simulation. 
*/

class Agent 
{
public:
    long double stackVal; // bankroll

    // struct to hold relevent information of a hand
    // can adjust relatively easily, and change the relevant methods (& constructor)
    // this will reset via its nullary constructor every time the table is cleared
    struct HandInfo {
        HandInfo();

        double wager; 

        unsigned int nHolding; // number of cards in hand
        unsigned int handVal; // soft/hard value of the hand, depending on if we have aces
        unsigned int nSoftAces; // number of aces that are (soft) counted as 11 in hand
        
        bool natBlackJack; // flag if we have natural blackjack

        Card firstCard; // for triggering pair flag
        bool holdingPair; // flag if we are holding a pair
        
    };

    // cython needs nullary constructor, and this is memeber class of simenginebj
    Agent(); 
    // construct an object by passing in pointers to the strategy template
    Agent(char* hrd, char* sft, char* splt, double* cnt); 

    bool stratInit; // keeps track of if the pointers below are actually pointing to strats

    void SetBJPayout(double d);

    void DealTargetHandler(const Card &dCard); // logic for recieving one card

    void DealObserveHandler(const Card &dCard); 

    char YieldAction(const Dealer &dealerRef); 

    void ClearHandler(const Dealer &dealerRef);

    void FreshShuffleHandler();

private:
    static constexpr unsigned int MAX_HSIZE = 21; // maximum hand size - 21 soft aces
    static constexpr unsigned int MAX_N_SPLITS = 3; // so we can stack allocate memory for hand information

    HandInfo hands[MAX_N_SPLITS + 1]; 
    unsigned int nActiveHands;
    unsigned int activeHandIdx;

    double* cntPtr; // pointer to find count values of various cards 
    char* hrdPtr; char* sftPtr; char* spltPtr;  // pointer to strategy input files
    
    double cntVal; // card count as dictated by count input

    // ------------------------------
    double BJ_PAYOUT; 


    // utils ---------------------------------------------------------------------
    
};

#endif