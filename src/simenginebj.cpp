#include "agent.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "simenginebj.hpp"
#include "types.hpp"

/*
Nullary constructor, to minimise issues when wrapping with Cython. 
From the Python API end, this will never be called, as the .__init__ method of 
the Python class will always call the one below. 
*/
SimEngineBJ::SimEngineBJ() {}

/*
Constructor which appropriately seeds the simShoe member object.
This will seed the pseudo random number generator, just this once. 
*/
SimEngineBJ::SimEngineBJ(unsigned int N_DECKS, double penen) : 
    activatedAgents(0),
    simShoe(N_DECKS, penen) 
{
    for (unsigned int i=0; i<MAX_N_AGENTS; ++i) {
        agentsActivateStatus[i] = false; 
    }
}

/*
Set dealer hit / stand on soft17. Default is false, see Dealer implementation.
*/
void SimEngineBJ::SetDealer17(bool b) 
{
    simDealer.HITSOFT17 = b;
}

/*

*/
void SimEngineBJ::SetAgent(
    unsigned int idx, 
    char* hrd, 
    char* sft, 
    char* splt, 
    double* cnt) 
{
    agents[idx] = Agent(hrd, sft, splt, cnt);
    agentsActivateStatus[idx] = true; 

    activatedAgents = 0; 
    for (unsigned int i=0; i<MAX_N_AGENTS; ++i) {
        if (agentsActivateStatus[i]) {
            activatedAgents += 1;
        }
    }
}

/*

*/
void SimEngineBJ::EventClear() 
{
    // let agents implement wager-settling logic before clearing other objects
    for (unsigned int i=0; i<activatedAgents; ++i) {
        agents[i].ClearHandler(simDealer);
    }
    
    simShoe.Clear(); 
    simDealer.ClearHandler(); 
}

/*

*/
template<typename targetType> void SimEngineBJ::EventDeal(targetType &target) 
{
    Card dCard = simShoe.Deal();
    target.DealTargetHandler(dCard);

    for (unsigned int i=0; i<activatedAgents; ++i) {
        agents[i].DealObserveHandler(dCard); 
    }
}

// explicit instantiations
template void SimEngineBJ::EventDeal<Agent>(Agent&); 
template void SimEngineBJ::EventDeal<Dealer>(Dealer&); 

/*

*/
void SimEngineBJ::EventQueryAgent(Agent &targetAgent) 
{
    // passes reference of the simulation dealer state
    if (targetAgent.YieldAction(simDealer) == ACTION::HIT) {
        EventDeal(targetAgent);
        EventQueryAgent(targetAgent); 
        return;
    }
    else /* queryResponse == ACTION::STAND */ {
        return;
    }
}

/*

*/
void SimEngineBJ::EventQueryDealer() 
{
    if (simDealer.YieldAction() == ACTION::HIT) {
        EventDeal(simDealer); 
        EventQueryDealer(); 
        return;
    }
    else /* queryResponse == ACTION::STAND */ {
        return; 
    }
}

/*

*/
ERR_CODE SimEngineBJ::RunSimulation(unsigned long long nIters) 
{
    nPlayed = 0; 
    shoeRounds = 0; 
    total = 0; 

    long curr; 

    if (activatedAgents == 0) {
        return ERR_CODE::NO_AGENT_STRAT;
    }
    
    // do a full shuffle of the shoe
    simShoe.FreshShuffleN(simShoe.N_CARDS); 

    // each iteration is playing one shoe
    for (unsigned long long i=0; i<nIters; ++i) { 
        // partial fresh shuffle - see Shoe implementation
        simShoe.FreshShuffleN(simShoe.N_UNTIL_CUT); 

        for (unsigned int i=0; i<activatedAgents; ++i) {
            agents[i].FreshShuffleHandler(); 
        }

        shoeRounds += 1; 
        curr = 0;

        // simulate until reshuffle condition is met - see Shoe implementation
        //
        // this is currently defined when a certain proportion of the Shoe has 
        // been expended, as per standard table rules
        //
        // each iteration represents 'one hand'
        while (!simShoe.needReshuffle) {
            EventClear(); 
            EventDeal(simDealer); 

            for (unsigned int i=0; i<activatedAgents; ++i) {
                EventDeal(agents[i]); 
                EventDeal(agents[i]);
            }

            for (unsigned int i=0; i<activatedAgents; ++i) {
                EventQueryAgent(agents[i]); 
            }

            EventQueryDealer(); 

            curr += 1; 
            total += 1; 
        }

        if (curr > nPlayed) {nPlayed = curr;}
    }

    return ERR_CODE::SUCCESS;
}
