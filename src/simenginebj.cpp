#include "simenginebj.hpp"
#include "strategyinput.hpp"


SimEngineBJ::SimEngineBJ(unsigned int ndecks, double penen) : 
    simShoe(ndecks, penen), _NDECKS(ndecks), _PENEN(penen), _BJPAYOUT(1.5) {;}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{simAgent = Agent(hrd, sft, splt, cnt);}

void SimEngineBJ::SetBJPayout(double payout) 
{_BJPAYOUT = payout;}




char SimEngineBJ::Test() {
    return simShoe.Deal().face;
}
