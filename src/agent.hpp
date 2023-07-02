#ifndef AGENT_H
#define AGENT_H 

#include "types.hpp"

class Agent 
{
public:
    Agent(); // cython needs nullary constructor, and this is memeber class of simenginebj

    // construct an object by passing in pointers to the strategy template
    Agent(char* hrd, char* sft, char* splt, double* cnt); 

    long long stackVal; // bankroll 

    unsigned int hrdVal; // hard value of the hand 
    unsigned int sftVal; // soft value of the hand
    unsigned int cntVal; // card count as dictated by count input

private:
    bool stratInit; // basically keeps track of if the pointers below are actually pointing to strats
    char* hrdPtr; char* sftPtr; char* spltPtr; double* cntPtr;  
};

#endif