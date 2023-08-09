#ifndef AGENT_H
#define AGENT_H 

#include "dealer.hpp"
#include "types.hpp"


/*
The simulation engine is actually designed to enforce a minimal set of 
arbitrary game mechanics. Most of the logic relating to valid player actions, 
wagering, and settling, is isolated in the agent class here. 

Various methods are called by the engine throughout a simulation round, and 
these can be modified fairly easily, to implement novel game logic. 

At its default configuration, the agent also shows how the example strategy and 
count template files can be used to easily and clearly configure a game strategy
of interest. 
*/
class Agent 
{
public:
    long double pnl; 

    // ensure that this always has a nullary constructor, which also serves as a
    // 'reset' method
    struct HandInfo {
        HandInfo();

        void Recieve(const Card &dCard);

        double wager; 

        unsigned int nCards;
        unsigned int handVal;
        unsigned int nSoftAces;
        
        Card first;
        Card second;
        bool natBlackJack;
        bool holdingPair;
    };

    Agent(); 

    Agent(char* hrd, char* sft, char* splt, double* cnt); 

    // explicit flag to indicate whether or not pointers have been configured
    // to a strategy template
    bool stratInit;

    void SetBJPayout(double d);

    void DealTargetHandler(const Card &dCard); 

    void DealObserveHandler(const Card &dCard); 

    ACTION YieldAction(const Dealer &dealerRef); 

    void ClearHandler(const Dealer &dealerRef);

    void FreshShuffleHandler();

private:
    // maximum hand size - 21 soft aces
    static constexpr unsigned int MAX_HSIZE = 21; 
    // so we can stack allocate memory for hand information
    static constexpr unsigned int MAX_N_SPLITS = 3; 

    HandInfo hands[MAX_N_SPLITS + 1]; 
    unsigned int nActiveHands;
    unsigned int hIdx; 

    double* cntPtr; 
    char* hrdPtr; 
    char* sftPtr; 
    char* spltPtr;
    
    double cntVal;

    double BJ_PAYOUT; 
};

#endif