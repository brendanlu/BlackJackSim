#ifndef AGENT_H
#define AGENT_H 

#include <array>

#include "communication.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"

static constexpr unsigned int MAX_N_HANDS = 4; 

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
    Agent(StratPackage s); 
    
    void ClearHandler(const Dealer &dealerRef);

    void DealTargetHandler(const Card &dCard); 
    void DealObserveHandler(const Card &dCard); 

    ACTION YieldAction(const Dealer &dealerRef); 

    void FreshShuffleHandler();

private:
    bool STRAT_INIT;

    bool BJ_INSTANT;
    double BJ_PAYOUT; 

    std::array<HandInfo, MAX_N_HANDS> hands;
    unsigned int newIdx;
    unsigned int currIdx; 

    StratPackage strat; 
    
    double cntVal;
};

#endif