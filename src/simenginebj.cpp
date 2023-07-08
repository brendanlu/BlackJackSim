#include "types.hpp"
#include "simenginebj.hpp"

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : 
    simShoe(N_DECKS, penen) {;}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{simAgent = Agent(hrd, sft, splt, cnt);}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

    simShoe.EfficientShuffle(0, simShoe.N_CARDS); // do a full shuffle of the shoe
    for (unsigned long long i=0; i<nIters; ++i) { // each iteration is playing one shoe
        simShoe.EfficientShuffle(0, simShoe.N_UNTIL_CUT); // partial fresh shuffle - see Shoe implementation
        simAgent.ShuffleHandler(); 

        

        // another event loop to play one hand

    }

    return ERR_CODE::SUCCESS;
}
