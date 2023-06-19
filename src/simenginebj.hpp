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

    double Test(); 

private:
    Shoe simShoe;
    Agent simAgent; 

    //void fetchHrdAction(); 
    //void fetchSftAction(); 
    //void fetchCntAction();

};


#endif