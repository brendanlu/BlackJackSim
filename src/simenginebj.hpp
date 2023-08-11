#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include <random>

#include "agent.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "types.hpp"

/*
A class which ochestrates the method calls of the simulation objects to 
implement correct game logic. 
*/
class SimEngineBJ
{
public: 
    Dealer simDealer;

    SimEngineBJ();
    SimEngineBJ(unsigned int N_DECKS, double penen);

    void SetDealer17(bool b);
    void SetBJPayout(double d); 
    void SetAgent(unsigned int idx, char* hrd, char* sft, 
                                char* splt, double* cnt);

    void EventClear();
    template<typename targetType> void EventDeal(targetType &target);
    void EventQueryAgent(Agent &targetAgent); 
    void EventQueryDealer(); 

    ERR_CODE RunSimulation(unsigned long long nIters);

    long nPlayed; 
    long shoeRounds; 
    long total;

private:
    static constexpr unsigned int MAX_N_AGENTS = 10; 

    Agent agents[MAX_N_AGENTS]; 
    bool agentsActivateStatus[MAX_N_AGENTS]; 
    unsigned int activatedAgents;

    Shoe<std::mt19937_64> simShoe;  
};

#endif