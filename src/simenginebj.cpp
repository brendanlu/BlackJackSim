#include "simenginebj.hpp"
#include "strategyinput.hpp"


SimEngineBJ::SimEngineBJ(unsigned int ndecks, double penen) : 
    simShoe(ndecks, penen) {;}


void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{
    simAgent = Agent(hrd, sft, splt, cnt);
}

char SimEngineBJ::Test() {
    return *(simAgent.hrdPtr);
}
