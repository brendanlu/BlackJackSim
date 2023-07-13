#include "agent.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"

Agent::HandInfo::HandInfo() : nHolding(0), handVal(0), nSoftAces(0), lastCard(BLANK_CARD), holdingPair(false) 
{}

Agent::Agent() : stratInit(false) // flag that we do not have pointers to strats yet
{} 

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), stackVal(0), cntVal(0) 
{}

double Agent::YieldWager() {

    return 10; 
}

// logic for recieving one card
void Agent::DealTargetHandler(const Card &dCard) {

    hInfo.nHolding += 1;

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {hInfo.nSoftAces += 1;}
    hInfo.handVal += dCard.val(); 
    if (hInfo.handVal > BJVAL && hInfo.nSoftAces > 0) { // revert soft count to hard count
        hInfo.handVal -= 10; // adjust ace value to 1 
        hInfo.nSoftAces -= 1; 
    }

    // track if we have pairs
    if (hInfo.nHolding == 1) {hInfo.lastCard = dCard;}
    else if (hInfo.nHolding == 2 && hInfo.lastCard.face == dCard.face) {hInfo.holdingPair = true;}
    
}

// logic for observing any general card being dealt out 
void Agent::DealObserveHandler(const Card &dCard) {
    // change internal running count
    cntVal += cntFromPtr(cntPtr, dCard.val()); 
}

/*
    HIT = 'H',
    STAND = 'S', 
    DOUBLE = 'D', 
    SPLIT = 'P', 
    SURRENDER = 'R'
*/
char Agent::YieldAction(const Dealer &dealerRef) {
    
    if (hInfo.handVal >= BJVAL) {
        return 'S'; 
    }
    else if (hInfo.holdingPair) {
        return spltActionFromPtr(spltPtr, hInfo.lastCard.val(), dealerRef.upCard.val());
    }
    else if (hInfo.nSoftAces > 0) {
        return sftActionFromPtr(sftPtr, hInfo.handVal, dealerRef.upCard.val()); 
    }
    else {
        return hrdActionFromPtr(hrdPtr, hInfo.handVal, dealerRef.upCard.val()); 
    }

    // return 'H'; 
}

void Agent::ClearHandler () {
    hInfo = HandInfo(); // reset hand information
}

void Agent::FreshShuffleHandler() {
    cntVal = 0;
}

