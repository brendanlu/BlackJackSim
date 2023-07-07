#include "simenginebj.hpp"
#include "strategyinput.hpp"

SimEngineBJ::SimEngineBJ(unsigned int ndecks, double penen) : 
    simShoe(ndecks, penen), _NDECKS(ndecks), _PENEN(penen), _BJPAYOUT(1.5) {;}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{simAgent = Agent(hrd, sft, splt, cnt);}

void SimEngineBJ::SetBJPayout(double payout) 
{_BJPAYOUT = payout;}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

    return ERR_CODE::SUCCESS;
}

int SimEngineBJ::Test() {
    return 1; 
}
