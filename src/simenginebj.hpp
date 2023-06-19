#ifndef SIMENGINE_H
#define SIMENGINE_H 

#include "shoe.hpp"
#include "agent.hpp"

class SimEngineBJ
{
public: 
    SimEngineBJ() {};

    SimEngineBJ(
        unsigned int ndecks,
        double penen, 
        char* hrd, 
        char* sft, 
        char* splt, 
        float* cnt
    );

    char Test(); 

private:
    Shoe simShoe;
    Agent simAgent; 

    char* hrdPtr; char* sftPtr; char* spltPtr; float* cntPtr;

    //void fetchHrdAction(); 
    //void fetchSftAction(); 
    //void fetchCntAction();

};


#endif