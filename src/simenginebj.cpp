#include "types.hpp"
#include "simenginebj.hpp"

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : 
    simShoe(N_DECKS, penen) {;}

void SimEngineBJ::setDealer17(bool b) {
    simDealer.HITSOFT17 = b;
}

SimEngineBJ::Dealer::Dealer() : HITSOFT17(false), handVal(0), nSoftAces(0) {}

void SimEngineBJ::Dealer::DealHandler(Card dCard) {
    if (dCard.face == 'A') {nSoftAces += 1;}
    handVal += dCard.val(); 
  
    if (handVal > BJVAL && nSoftAces > 0) { // revert soft count to hard count
    handVal -= 10; // adjust ace value to 1 
    nSoftAces -= 1; 
    }
}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{simAgent = Agent(hrd, sft, splt, cnt);}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

    simShoe.EfficientShuffle(0, simShoe.N_CARDS); // do a full shuffle of the shoe
    for (unsigned long long i=0; i<nIters; ++i) { // each iteration is playing one shoe
        
        // reshuffle the shoe --------------------------------------------------------------------
        simShoe.EfficientShuffle(0, simShoe.N_UNTIL_CUT); // partial fresh shuffle - see Shoe implementation
        simAgent.ShuffleHandler(); 

        // play the shoe until reshuffle triggered by drawing cut card
        while (!simShoe.needReshuffle) 
        {
            // initial deal out --------------------
            simShoe.Deal(simDealer);
            simShoe.Deal(simAgent);
            simShoe.Deal(simAgent);

            

        }

    }

    return ERR_CODE::SUCCESS;
}
