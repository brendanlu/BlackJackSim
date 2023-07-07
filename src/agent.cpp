#include "types.hpp"
#include "agent.hpp"
#include "strategyinput.hpp"

Agent::Agent() : stratInit(false) {} // flag that we do not have pointers to strats yet

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), cntVal(0), 
hInfo({
    0, // handVal
    0, // nSoftAces
    0, // nCards
    '0', // lastFace
    false // holdingPair
}) 
{}

// logic for recieving one card
void Agent::dealHandler(Card dCard) {
    // change internal running count
    cntVal += cntFromPtr(cntPtr, dCard.val()); 

    hInfo.nCards += 1;

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {hInfo.nSoftAces += 1;}
    hInfo.handVal += dCard.val(); 
    if (hInfo.handVal > BJVAL && hInfo.nSoftAces > 0) { // revert soft count to hard count
        hInfo.handVal -= 10; // adjust ace value to 1 
        hInfo.nSoftAces -= 1; 
    }

    // track if we have pairs
    if (hInfo.nCards == 1) {hInfo.lastFace = dCard.face;}
    else if (hInfo.nCards == 2) 
    {
        if (hInfo.lastFace == dCard.face) {hInfo.holdingPair = true;}
    }
}