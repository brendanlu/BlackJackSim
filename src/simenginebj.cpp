#include "agent.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "simenginebj.hpp"
#include "types.hpp"

SimEngineBJ::SimEngineBJ() {} // this is just for Cython; the Python constructor will always call the one below

SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : 
    simShoe(N_DECKS, penen), 
    activatedAgents(0) 
{
    for (unsigned int i=0; i<MAX_N_AGENTS; ++i) {
        agentsActivateStatus[i] = false; 
    }
}

void SimEngineBJ::SetDealer17(bool b) {simDealer.HITSOFT17 = b;}

// these are pointers which come in from the Python interface
//      so we will have to sure in the Python level they are valid
void SimEngineBJ::SetAgentStrat(unsigned int agentID, char* hrd, char* sft, char* splt, double* cnt) {
    agents[agentID] = Agent(hrd, sft, splt, cnt);
    agentsActivateStatus[agentID] = true; 

    activatedAgents = 0; 
    for (unsigned int i=0; i<MAX_N_AGENTS; ++i) {
        if (agentsActivateStatus[i]) {activatedAgents += 1;}
    }
}

void SimEngineBJ::SetBJPayout(double d) {
    for (unsigned int i=0; i<MAX_N_AGENTS; ++i) {
        agents[i].SetBJPayout(d);
    }
}

void SimEngineBJ::SetAgentStack(unsigned int agentID, long double sv) {
    agents[agentID].stackVal = sv;
}

template<typename targetType> void SimEngineBJ::EventDeal(targetType &target) {
    Card dCard = simShoe.Deal();
    target.DealTargetHandler(dCard);

    for (unsigned int i=0; i<activatedAgents; ++i) {
        agents[i].DealObserveHandler(dCard); 
    }
}
template void SimEngineBJ::EventDeal<Agent>(Agent&); 
template void SimEngineBJ::EventDeal<Dealer>(Dealer&); 

ERR_CODE SimEngineBJ::EventQueryAgent(Agent &targetAgent) {
    char queryResponse = targetAgent.YieldAction(simDealer); // give reference of Dealer state

    if (queryResponse == static_cast<char>(ACTION::HIT)) {
        EventDeal(targetAgent);
        EventQueryAgent(targetAgent); 
        return ERR_CODE::SUCCESS; 
    }
    else if (queryResponse == static_cast<char>(ACTION::STAND)) {
        return ERR_CODE::SUCCESS;
    }
    else {
        // will just return error code and do nothing (basically treats it as a 'stand' message)
        return ERR_CODE::INVALID_ACTION;
    }
}

void SimEngineBJ::EventQueryDealer() {
    if (simDealer.YieldAction() == ACTION::HIT) {
        EventDeal(simDealer); 
        EventQueryDealer(); 
    }
}

void SimEngineBJ::EventClear() {
    simShoe.Clear(); 
    simDealer.ClearHandler(); 

    for (unsigned int i=0; i<activatedAgents; ++i) {
        agents[i].ClearHandler(simDealer);
    }
}

ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) {
    // The Python constructor will appropriately construct the Shoe
    // So at this point it will be populated and ready to go
    if (activatedAgents == 0) {return ERR_CODE::NO_AGENT_STRAT;}

    simShoe.FreshShuffleN(simShoe.N_CARDS); // do a full shuffle of the shoe
    for (unsigned long long i=0; i<nIters; ++i) { // each iteration is playing one shoe
        
        // reshuffle the shoe --------------------------------------------------------------------
        simShoe.FreshShuffleN(simShoe.N_UNTIL_CUT); // partial fresh shuffle - see Shoe implementation
        for (unsigned int i=0; i<activatedAgents; ++i) {
            agents[i].FreshShuffleHandler(); 
        }

        // play the shoe until reshuffle triggered by drawing cut card
        while (!simShoe.needReshuffle) 
        {
        // each iteration is a hand  -----------------------------------------------------------------

            // clear out table and settle bets ---------------------------------
            EventClear(); 

            // initial deal out ------------------------------------------------
            //      dealer up card
            EventDeal(simDealer); 
            //      player gets two cards
            for (unsigned int i=0; i<activatedAgents; ++i) {
                EventDeal(agents[i]); 
                EventDeal(agents[i]);
            }
            // action ----------------------------------------------------------
            //      player actions
            for (unsigned int i=0; i<activatedAgents; ++i) {
                EventQueryAgent(agents[i]); 
            }
            //      dealer takes cards 
            EventQueryDealer(); 
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
    agents[0].DealTargetHandler(simShoe.Deal());
    agents[0].DealTargetHandler(simShoe.Deal());

    cout << "Agent deal is not broken yet \n\n";

    simDealer.DealTargetHandler(simShoe.Deal());
    
    cout << "Dealer deal is not broken yet \n\n"; 


} 