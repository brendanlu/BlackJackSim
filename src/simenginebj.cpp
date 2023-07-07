#include "simenginebj.hpp"
#include "strategyinput.hpp"

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : 
    simShoe(N_DECKS, penen), _N_DECKS(N_DECKS), _PENEN(penen), _BJPAYOUT(1.5) {;}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{simAgent = Agent(hrd, sft, splt, cnt);}

void SimEngineBJ::SetBJPayout(double payout) 
{_BJPAYOUT = payout;}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

    simShoe.Shuffle(); // do a full shuffle of the shoe
    for (unsigned long long i=0; i<nIters; ++i) {
        ; // play blackjack shoe
    }

    return ERR_CODE::SUCCESS;
}
