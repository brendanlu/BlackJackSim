#include "types.hpp"
#include "agent.hpp"
#include "strategyinput.hpp"

Agent::Agent() : stratInit(false) {} // flag that we do not have pointers to strats yet

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), cntVal(0), 
hInfo({0, 0, 0}) 
{}

// logic for recieving one card
void Agent::dealHandler(Card dCard) {
    cntVal += cntFromPtr(cntPtr, dCard.val()); // change internal running count

    hInfo.nCards += 1;
    if (dCard.face == 'A') {hInfo.nSoftAces += 1;}
    hInfo.handVal += dCard.val(); 
    if (hInfo.handVal > BJVAL && hInfo.nSoftAces > 0) { // revert soft count to hard count
        hInfo.handVal -= 10; // adjust ace value to 1 
        hInfo.nSoftAces -= 1; 
    }
}