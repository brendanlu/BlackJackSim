#include "agent.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"

Agent::HandInfo::HandInfo() : nHolding(0), handVal(0), nSoftAces(0), lastCard(BLANK_CARD), holdingPair(false), blackJack(false)
{}

Agent::Agent() : stratInit(false) // flag that we do not have pointers to strats yet
{} 

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), stackVal(0), cntVal(0), activeHand(0)
{}

double Agent::YieldWager() {

    return 10; 
}

// logic for recieving one card
void Agent::DealTargetHandler(const Card &dCard) {

    hands[activeHand].nHolding += 1;

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {hands[activeHand].nSoftAces += 1;}
    hands[activeHand].handVal += dCard.val(); 
    if (hands[activeHand].handVal > BJVAL && hands[activeHand].nSoftAces > 0) { // revert soft count to hard count
        hands[activeHand].handVal -= 10; // adjust ace value to 1 
        hands[activeHand].nSoftAces -= 1; 
    }

    // track if we have pairs
    if (hands[activeHand].nHolding == 1) {hands[activeHand].lastCard = dCard;}
    else if (hands[activeHand].nHolding == 2 && hands[activeHand].lastCard.face == dCard.face) {hands[activeHand].holdingPair = true;}

    // check for instant blackjack
    
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
    
    if (hands[activeHand].handVal >= BJVAL) {
        return 'S'; 
    }
    else if (hands[activeHand].holdingPair) {
        return spltActionFromPtr(spltPtr, hands[activeHand].lastCard.val(), dealerRef.upCard.val());
    }
    else if (hands[activeHand].nSoftAces > 0) {
        return sftActionFromPtr(sftPtr, hands[activeHand].handVal, dealerRef.upCard.val()); 
    }
    else {
        return hrdActionFromPtr(hrdPtr, hands[activeHand].handVal, dealerRef.upCard.val()); 
    }
}

void Agent::ClearHandler () {

    hands[activeHand] = HandInfo(); // reset hand information
}

void Agent::FreshShuffleHandler() {
    cntVal = 0;
}

