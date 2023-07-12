#include "types.hpp"
#include "simenginebj.hpp"
#include "shoe.hpp"
#include "agent.hpp"
#include "dealer.hpp"

SimEngineBJ::SimEngineBJ() {}

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : simShoe(N_DECKS, penen) {}

void SimEngineBJ::SetDealer17(bool b) {
    simDealer.HITSOFT17 = b;
}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
// these are pointers which come in from the Python interface
//      so we will have to do valid checks there
{simAgent = Agent(hrd, sft, splt, cnt);}

template<typename targetType> void SimEngineBJ::EventDeal(targetType &target) {
    Card dCard = simShoe.Deal();
    target.DealTargetHandler(dCard);

    // later we can loop over multiple agents here
    simAgent.DealObserveHandler(dCard); 
}
template void SimEngineBJ::EventDeal<Agent>(Agent&); 
template void SimEngineBJ::EventDeal<Dealer>(Dealer&); 

ERR_CODE SimEngineBJ::EventQueryAgent(Agent &targetAgent) {
    ACTION queryResponse = targetAgent.YieldAction(); // give reference of Dealer state

    if (queryResponse == ACTION::HIT) {
        EventDeal(targetAgent);
        return ERR_CODE::SUCCESS; 
    }
    else if (queryResponse == ACTION::STAND) {
        return ERR_CODE::SUCCESS;
    }
    else if (queryResponse == ACTION::DOUBLE) {
        return ERR_CODE::SUCCESS;
    }
    else if (queryResponse == ACTION::SPLIT) {
        return ERR_CODE::SUCCESS;
    }
    else if (queryResponse == ACTION::SURRENDER) {
        return ERR_CODE::SUCCESS;
    }
    else {
        return ERR_CODE::INVALID_ACTION;
    }
}

void SimEngineBJ::EventClear() {
    simShoe.Clear(); 
    simAgent.ClearHandler();
    simDealer.ClearHandler(); 
}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

    simShoe.FreshShuffleN(simShoe.N_CARDS); // do a full shuffle of the shoe
    for (unsigned long long i=0; i<nIters; ++i) { // each iteration is playing one shoe
        
        // reshuffle the shoe --------------------------------------------------------------------
        simShoe.FreshShuffleN(simShoe.N_UNTIL_CUT); // partial fresh shuffle - see Shoe implementation
        simAgent.FreshShuffleHandler(); 

        // play the shoe until reshuffle triggered by drawing cut card
        while (!simShoe.needReshuffle) 
        {
        // each iteration is a hand  -----------------------------------------------------------------

            // places bets -----------------------------------------------------


            // initial deal out ------------------------------------------------
            //      dealer up card
            EventDeal(simDealer); 

            //      player gets two cards
            EventDeal(simAgent); 
            EventDeal(simAgent); 

            // agent action ----------------------------------------------------
            EventQueryAgent(simAgent);
            
            // settle bets -----------------------------------------------------


            // clear the table -------------------------------------------------
            EventClear();
        }
    }

    return ERR_CODE::SUCCESS;
}

// testing ---------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
#include <iostream>
using std::cout;

void SimEngineBJ::Test2() {

}

void SimEngineBJ::Test() {
    cout << "Program start \n"; 
    cout << "Playing with: " << simShoe.N_CARDS << " cards \n\n";

    simShoe.Display();
    cout << "\n\n";
    simShoe.FreshShuffleN(10);
    simShoe.Display();
    cout << "\n\n";



    Card tryCard = {'J', 'H'};
    cout << "Testing value of card J: " << tryCard.val() << "\n\n";

    cout << "Dealing to internals now \n\n"; 
    // simShoe.Deal(simDealer);
    simAgent.DealTargetHandler(simShoe.Deal());
    simAgent.DealTargetHandler(simShoe.Deal());

    cout << "Agent deal is not broken yet \n\n";

    simDealer.DealTargetHandler(simShoe.Deal());
    
    cout << "Dealer deal is not broken yet \n\n"; 


} 