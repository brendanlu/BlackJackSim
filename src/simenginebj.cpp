#include "simenginebj.hpp"
#include "strategyinput.hpp"


SimEngineBJ::SimEngineBJ(unsigned int ndecks, double penen) : 
    simShoe(ndecks, penen) {;}


void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{
    simAgent = Agent(hrd, sft, splt, cnt);
}

double SimEngineBJ::Test() {
    return cntFromPtr(simAgent.cntPtr, 11);
    // return *(simAgent.hrdPtr + 63);
}
