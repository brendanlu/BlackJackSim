#include "types.hpp"
#include "simenginebj.hpp"
#include "shoe.hpp"
#include "agent.hpp"

// the compiler needs to find a nullary constructor for the nested Dealer struct 
//      otherwise the SimEngine constructor will not work 
SimEngineBJ::Dealer::Dealer() : HITSOFT17(false), handVal(0), nSoftAces(0), upCard(BLANK_CARD) {}

SimEngineBJ::SimEngineBJ() {}

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : simShoe(N_DECKS, penen) {}

void SimEngineBJ::Dealer::DealHandler(Card dCard) {
    if (dCard.face == 'A') {nSoftAces += 1;}
    handVal += dCard.val(); 
  
    if (handVal > BJVAL && nSoftAces > 0) { // revert soft count to hard count
    handVal -= 10; // adjust ace value to 1 
    nSoftAces -= 1; 
    }

    if (upCard == BLANK_CARD) {upCard == dCard;}
}

void SimEngineBJ::Dealer::ClearHandler() {
    upCard = BLANK_CARD;
}

void SimEngineBJ::SetDealer17(bool b) {
    simDealer.HITSOFT17 = b;
}

void SimEngineBJ::SetAgentStrat(char* hrd, char* sft, char* splt, double* cnt)
// these are pointers which come in from the Python interface
//      so we will have to do valid checks there
{simAgent = Agent(hrd, sft, splt, cnt);}

ERR_CODE SimEngineBJ::QueryAgent(Agent &targetAgent) {
    ACTION queryResponse = targetAgent.YieldAction(); // give reference of Dealer state

    if (queryResponse == ACTION::HIT) {
        targetAgent.DealHandler(simShoe.Deal());
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

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    // if (!simAgent.stratInit) {return ERR_CODE::NO_AGENT_STRAT;}

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
            simDealer.DealHandler(simShoe.Deal()); 
            //      player gets two cards
            simAgent.DealHandler(simShoe.Deal());
            simAgent.DealHandler(simShoe.Deal());

            // agent action ----------------------------------------------------
            QueryAgent(simAgent);
            
            // settle bets -----------------------------------------------------


            // clear the table -------------------------------------------------
            simShoe.Clear(); 
            simAgent.ClearHandler();
            simDealer.ClearHandler(); 
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
    simAgent.DealHandler(simShoe.Deal());
    simAgent.DealHandler(simShoe.Deal());

    cout << "Agent deal is not broken yet \n\n";

    simDealer.DealHandler(simShoe.Deal());
    
    cout << "Dealer deal is not broken yet \n\n"; 


} 