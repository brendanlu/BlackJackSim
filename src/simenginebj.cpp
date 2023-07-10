#include "types.hpp"
#include "simenginebj.hpp"

SimEngineBJ::Dealer::Dealer() : HITSOFT17(false), handVal(0), nSoftAces(0) {}

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : 
    simShoe(N_DECKS, penen) {;}

void SimEngineBJ::Dealer::DealHandler(Card dCard) {
    if (dCard.face == 'A') {nSoftAces += 1;}
    handVal += dCard.val(); 
  
    if (handVal > BJVAL && nSoftAces > 0) { // revert soft count to hard count
    handVal -= 10; // adjust ace value to 1 
    nSoftAces -= 1; 
    }
}

void SimEngineBJ::setDealer17(bool b) {
    simDealer.HITSOFT17 = b;
}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
{simAgent = Agent(hrd, sft, splt, cnt);}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

    simShoe.FreshShuffleN(simShoe.N_CARDS); // do a full shuffle of the shoe
    for (unsigned long long i=0; i<nIters; ++i) { // each iteration is playing one shoe
        
        // reshuffle the shoe --------------------------------------------------------------------
        simShoe.FreshShuffleN(simShoe.N_UNTIL_CUT); // partial fresh shuffle - see Shoe implementation
        simAgent.ShuffleHandler(); 

        // play the shoe until reshuffle triggered by drawing cut card
        while (!simShoe.needReshuffle) 
        {
            // initial deal out --------------------
            // simShoe.Deal(simDealer);
            simShoe.Deal(simAgent);
            simShoe.Deal(simAgent);

            

        }

    }

    return ERR_CODE::SUCCESS;
}


#include <iostream>
using std::cout;

void SimEngineBJ::Test() {
    cout << "Program start \n"; 
    Shoe new_shoe(1, 0.25);
    new_shoe.Display();
    cout << "\n\n";
    new_shoe.FreshShuffleN(10);
    new_shoe.Display();
    cout << "\n\n";

    Card tryCard = {'3', 'D'};
    cout << tryCard.val() << "\n\n";

    SimEngineBJ tryEngine(2, 0.25); 

    Agent simAgent;

    cout << "Init stuff works \n\n"; 
    // new_shoe.Deal(simDealer);
    new_shoe.Deal(simAgent);
    new_shoe.Deal(simAgent);

    cout << "Agent deal is not broken yet \n\n";
} 