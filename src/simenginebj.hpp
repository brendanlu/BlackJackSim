#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include "shoe.hpp"
#include "agent.hpp"

class SimEngineBJ
{
public: 
    SimEngineBJ() {}; // Cython needs nullary constructor to init to stack
    SimEngineBJ(unsigned int ndecks, double penen);

    void SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt);
    void SetBJPayout(double payout);
    

    char Test(); 
    // for Python API user to be able to call
    unsigned int _NDECKS; 
    double _PENEN; 
    double _BJPAYOUT; 

private:
    Shoe simShoe;
    Agent simAgent; 

};


#endif