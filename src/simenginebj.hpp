#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include "shoe.hpp"
#include "agent.hpp"

// a collection of some error codes
enum class ERR_CODE : int {
    NO_AGENT_STRAT = 1, 
    SUCCESS = 100, 
};

class SimEngineBJ
{
public: 
    SimEngineBJ() {}; // Cython needs nullary constructor to init to stack...
    // ... but this wil always be called from the Python init method
    SimEngineBJ(unsigned int ndecks, double penen);

    void SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt);
    void SetBJPayout(double payout);
    
    ERR_CODE RunSimulation(unsigned long long nIters);

    // for Python API user to be able to call
    unsigned int _NDECKS; 
    double _PENEN; 
    double _BJPAYOUT; 

private:
    Shoe simShoe;
    Agent simAgent; 

};


#endif