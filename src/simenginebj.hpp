#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include "types.hpp"
#include "shoe.hpp"
#include "agent.hpp"

/*
This SimEngine uses the Shoe and Agent objects, which have been designed for Blackjack,
This class actually implements the game logic by ochestrating the method calls. 
*/

class SimEngineBJ
{
public: 
    SimEngineBJ() {}; // Cython needs nullary constructor to init to stack...
    // ... but this second constructor wil always be called from the Python init method
    SimEngineBJ(unsigned int N_DECKS, double penen);

    void SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt);
    
    ERR_CODE RunSimulation(unsigned long long nIters);

private:
    Shoe simShoe;
    Agent simAgent; 

};


#endif