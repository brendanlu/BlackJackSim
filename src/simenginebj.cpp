#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "agent.hpp"
#include "card.hpp"
#include "communication.hpp"
#include "dealer.hpp"
#include "shoe.hpp"
#include "simenginebj.hpp"
#include "strategyinput.hpp"

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
    simShoe(N_DECKS, penen),
    nAgents(0)
{
    simLog.InitLogFile("LOG.csv"); 
}

/*
Constructor which appropriately constructs all of the simulation objects and 
the game state. 
*/
SimEngineBJ::SimEngineBJ(InitPackage init) :
    simShoe(init.nDecks, init.shoePenentration),
    simDealer(init.dealer17),
    nAgents(init.nAgents)
{
    if (nAgents > MAX_N_AGENTS) {
        nAgents = MAX_N_AGENTS; 
    }

    for (unsigned int i=0; i<nAgents; ++i) {
        agents[i] = Agent(init.strats[i]);
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
}

/*

*/
void SimEngineBJ::SetLogFile(const std::string& filename) 
{
    simLog.InitLogFile(filename); 
}

/*

*/
void SimEngineBJ::EventFreshShuffle(unsigned int n) 
{
    simShoe.FreshShuffleN(n); 

    for (unsigned int i=0; i<nAgents; ++i) {
        agents[i].FreshShuffleHandler(); 
    }
    
    simLog.FreshShuffleHandler(); 
}

/*

*/
void SimEngineBJ::EventClear() 
{
    // let agents implement wager-settling logic before clearing other objects
    for (unsigned int i=0; i<nAgents; ++i) {
        agents[i].ClearHandler(simDealer);
    }
    
    simShoe.Clear(); 
    simDealer.ClearHandler(); 

    simLog.Clearhandler(); 
}

/*

*/
template<typename targetType> void SimEngineBJ::EventDeal(targetType &target) 
{
    Card dCard = simShoe.Deal();
    target.DealTargetHandler(dCard);

    for (unsigned int i=0; i<nAgents; ++i) {
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
void SimEngineBJ::RunSimulation(unsigned long long nIters) 
{
    auto start = std::chrono::system_clock::now();

    // to avoid making mistakes writing messages
    std::string CONTEXTSTRING1 = "Simulation Status"; 

    // this is currently redundant, as the log destination is configured in the
    // constructor
    //
    // but it may be useful later if this gets done differently
    if (!simLog) {
        simLog.InitLogFile("ERROR.csv"); 
        simLog.WriteRow(
            LOG_TYPE::ENGINE, 
            CONTEXTSTRING1, 
            "ERROR - LOG FILE DESINATION NOT CONFIGURED"
        );
        simLog.ManualFlush(); 
        return; 
    }
    else {
        simLog.WriteRow(
            LOG_TYPE::ENGINE, 
            CONTEXTSTRING1,
            "COMMENCING " 
                + std::to_string(nIters)
                + " SHOE SIMULATIONS"
        );
    }
    
    // do a full shuffle of the shoe
    //
    // directly call the shoe method to avoid affecting log
    // the first iteration of the simulation will reshuffle again anyway
    simShoe.FreshShuffleN(simShoe.N_CARDS); 

    // each iteration is playing one shoe
    for (unsigned long long i=0; i<nIters; ++i) {
        // partial fresh shuffle - see Shoe implementation
        EventFreshShuffle(simShoe.N_UNTIL_CUT);

        // simulate until reshuffle condition is met - see Shoe implementation
        //
        // this is currently defined when a certain proportion of the Shoe has 
        // been expended, as per standard table rules
        //
        // each iteration represents 'one hand'
        while (!simShoe.needReshuffle) {
            EventClear(); 
            EventDeal(simDealer); 

            for (unsigned int i=0; i<nAgents; ++i) {
                EventDeal(agents[i]); 
                EventDeal(agents[i]);
            }

            for (unsigned int i=0; i<nAgents; ++i) {
                EventQueryAgent(agents[i]); 
            }

            EventQueryDealer(); 
        }

        simLog.WriteRow(
            LOG_TYPE::ENGINE, 
            "Shoe Completed", 
            ""
        );
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    );

    simLog.WriteRow(
        LOG_TYPE::ENGINE, 
        CONTEXTSTRING1,
        "SUCCESS - SIMULATION COMPLETED IN " 
            + std::to_string(elapsed.count())
            + "ms"
    );

    simLog.ManualFlush();

    return;
}
