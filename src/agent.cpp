#include "agent.hpp"
#include "types.hpp"
#include "strategyinput.hpp"

Agent::HandInfo::HandInfo() : nHolding(0), handVal(0), nSoftAces(0), lastFace('0'), holdingPair(false) 
{}

Agent::Agent() : stratInit(false) // flag that we do not have pointers to strats yet
{} 

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), cntVal(0) 
{}

// logic for recieving one card
void Agent::DealHandler(Card dCard) {
    // change internal running count
    // cntVal += cntFromPtr(cntPtr, dCard.val()); 

    hInfo.nHolding += 1;

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {hInfo.nSoftAces += 1;}
    hInfo.handVal += dCard.val(); 
    if (hInfo.handVal > BJVAL && hInfo.nSoftAces > 0) { // revert soft count to hard count
        hInfo.handVal -= 10; // adjust ace value to 1 
        hInfo.nSoftAces -= 1; 
    }

    // track if we have pairs
    if (hInfo.nHolding == 1) {hInfo.lastFace = dCard.face;}
    else if (hInfo.nHolding == 2 && hInfo.lastFace == dCard.face) {hInfo.holdingPair = true;}
}

void Agent::FreshShuffleHandler() {
    cntVal = 0;
}

ACTION Agent::YieldAction() {
    return ACTION::STAND;
}

void Agent::ClearHandler () {
    hInfo = HandInfo(); // reset hand information
}