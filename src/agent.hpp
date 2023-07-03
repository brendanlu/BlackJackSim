#ifndef AGENT_H
#define AGENT_H 

#include "types.hpp"

class Agent 
{
public:
    Agent(); // cython needs nullary constructor, and this is memeber class of simenginebj

    // construct an object by passing in pointers to the strategy template
    Agent(char* hrd, char* sft, char* splt, double* cnt); 

    void dealHandler(Card dCard);

    long long stackVal; // bankroll 
    bool stratInit; // keeps track of if the pointers below are actually pointing to strats

private:
    char* hrdPtr; char* sftPtr; char* spltPtr; double* cntPtr;  
    unsigned int cntVal; // card count as dictated by count input

    HandInfo hInfo; 
};

#endif