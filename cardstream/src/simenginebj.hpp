#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "agent.hpp"
#include "card.hpp"
#include "communication.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "strategyinput.hpp"

/*

*/
struct InitPackage
{
    unsigned int nDecks;
    double shoePenentration;

    bool dealer17;

    unsigned int nAgents;
    // pointer to the head of an array of StratPackage 's of size nAgents 
    StratPackage *strats; 
};

/*
A class which ochestrates the method calls of the simulation objects to 
implement correct game logic. Maintains a connection to a Logger through a 
shared pointer. 

TO BE WRAPPED IN CYTHON, THIS CLASS MUST BE COPYABLE. 

Cython wrapping observations ---------------------------------------------------
Cython creates a nullary constructed object first; our Python .__init__ 
will then create a temporary object using a different consutrctor, and copy it  
into the local stack object through = assignment. 
--------------------------------------------------------------------------------
*/
class SimEngineBJ
{
public: 
    SimEngineBJ();
    SimEngineBJ(unsigned int N_DECKS, double penen);
    SimEngineBJ(InitPackage init);

    void SetAgent(unsigned int idx, char* hrd, char* sft, 
                                char* splt, double* cnt);
    void SetDealer17(bool b);
    void SetLogFile(std::string filename); 
    void SetSocketConnection(std::string ip, int port); 
    void SetLogLevel(int ll);

    void RunSimulation(unsigned long nIters);

private:
    static constexpr unsigned int MAX_N_AGENTS = 10; 

    std::shared_ptr<Logger> simLog;
    std::string LOGFNAME;
    std::string SOCKETIP;
    int SOCKETPORT;
    int LOGLEVEL; 

    Shoe<std::mt19937_64> simShoe;
    
    Dealer simDealer;

    unsigned int nAgents;
    std::array<Agent, MAX_N_AGENTS> simAgents;

    void InitLoggerInstance(); 

    void EventFreshShuffle(unsigned int n); 

    void EventClear();

    template<typename targetType> 
    void EventDeal(targetType &target);

    void EventQueryAgent(Agent &targetAgent); 

    void EventQueryDealer(); 
};

#endif