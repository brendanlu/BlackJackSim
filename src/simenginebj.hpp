#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include "agent.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "types.hpp"

/*
This SimEngine uses the Shoe and Agent objects, which have been designed for Blackjack,
This class actually implements the game logic by ochestrating the method calls. 

It also stores all the relevant simulation information, and is designed for use in Python through a Cython class wrapper
    Users in Python can query and find useful simulation statistics. 
*/

class SimEngineBJ
{
public: 
    Dealer simDealer;

    SimEngineBJ(); // Cython needs nullary constructor to init to stack...
    // ... but this second constructor wil always be called from the Python init method
    SimEngineBJ(unsigned int N_DECKS, double penen);

    void SetDealer17(bool b);

    void SetAgentStrat(unsigned int agentID, char* hrd, char* sft, char* splt, double* cnt);

    void SetAgentStack(unsigned int agentID, long double sv);  

    template<typename targetType> void EventDeal(targetType &target);

    ERR_CODE EventQueryAgent(Agent &targetAgent); 
 
    void EventQueryDealer(); 

    void EventClear();

    ERR_CODE RunSimulation(unsigned long long nIters);

    void Test(); 
    void Test2(); 

private:
    static constexpr unsigned int MAX_N_AGENTS = 10; // stack allocated; maximum number of players in simulation

    Shoe simShoe;

    Agent agents[MAX_N_AGENTS]; 
    bool agentsActivateStatus[MAX_N_AGENTS]; 
    unsigned int activatedAgents;
    
};


#endif