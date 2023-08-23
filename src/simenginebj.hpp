#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include <array>
#include <iostream>
#include <random>

#include "agent.hpp"
#include "communication.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "strategyinput.hpp"
#include "types.hpp"

static constexpr unsigned int MAX_N_AGENTS = 10; 

/*

*/
struct InitPackage
{
    unsigned int nDecks;
    double shoePenentration;

    bool dealer17;

    unsigned int nAgents; 
    StratPackage strats[MAX_N_AGENTS]; 
};

/*
A class which ochestrates the method calls of the simulation objects to 
implement correct game logic. 
*/
class SimEngineBJ
{
public: 
    SimEngineBJ();
    SimEngineBJ(unsigned int N_DECKS, double penen);
    SimEngineBJ(InitPackage init);

    void SetDealer17(bool b);
    void SetAgent(unsigned int idx, char* hrd, char* sft, 
                                char* splt, double* cnt);

    void EventClear();

    template<typename targetType> 
    void EventDeal(targetType &target);

    void EventQueryAgent(Agent &targetAgent); 

    void EventQueryDealer(); 

    void RunSimulation(unsigned long long nIters);

    long nPlayed; 
    long shoeRounds; 
    long total;

private:
    Shoe<std::mt19937_64> simShoe;
    Dealer simDealer;

    unsigned int nAgents;
    std::array<Agent, MAX_N_AGENTS> agents;
};

#endif