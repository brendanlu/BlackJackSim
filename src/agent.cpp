#include "agent.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"

Agent::HandInfo::HandInfo() : nHolding(0), handVal(0), nSoftAces(0), firstCard(BLANK_CARD), holdingPair(false), blackJack(false)
{}

Agent::Agent() : stratInit(false) // flag that we do not have pointers to strats yet
{} 

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), stackVal(0), cntVal(0), nActivehands(1), activeHand(0)
{}

// logic for recieving one card into the active hand
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
    if (hands[activeHand].nHolding == 1) {hands[activeHand].firstCard = dCard;}
    else if (hands[activeHand].nHolding == 2 && hands[activeHand].firstCard.face == dCard.face) {hands[activeHand].holdingPair = true;}

    // check for instant blackjack
    if (hands[activeHand].handVal == BJVAL && hands[activeHand].nHolding == 2) {
        if (hands[activeHand].firstCard.face == 'A') {
            hands[activeHand].blackJack = true; 
        }
        else if (hands[activeHand].firstCard.val() == 10) {
            hands[activeHand].blackJack = true;
        }
    }
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
    /*
    One can design any number of actions, restricted by the number of valid unique char codes. 

    The SimEngine will only process 'H' and 'S' codes; the rest of these actions
        one must implement the logic themselves in this agent class here. 
    */
    char internalAction; 

    // perform action lookup
    if (hands[activeHand].handVal >= BJVAL) { 
        // we do nothing if we bust; changing this will break game logic - so do with caution
        //      it will also break the lookup via the preconfigured templates, through misindexing problems
        return 'S'; 
    }
    else if (hands[activeHand].holdingPair) {
        internalAction = spltActionFromPtr(spltPtr, hands[activeHand].firstCard.val(), dealerRef.upCard.val());
    }
    else if (hands[activeHand].nSoftAces > 0) {
        internalAction = sftActionFromPtr(sftPtr, hands[activeHand].handVal, dealerRef.upCard.val()); 
    }
    else {
        internalAction = hrdActionFromPtr(hrdPtr, hands[activeHand].handVal, dealerRef.upCard.val()); 
    }

    if (internalAction == 'H' || internalAction == 'S') { // we immediately action these
        return internalAction;
    }
    // following actions require some processing on the Agent side before we send a message to the simulation engine
    else if (internalAction == 'D') {
        return 'H';
    }
    else if (internalAction == 'P') {
        // this will be fairly complex to implement
        return 'S'; 
    }
    else if (internalAction == 'R') {
        return 'S';
    }
    else {
        return 'S'; 
    }
}

void Agent::ClearHandler () {
    for (unsigned int i=0; i<nActivehands; ++i) {
        hands[i] = HandInfo(); // reset hand information
    }
    nActivehands = 1; 
    activeHand = 0; 
}

void Agent::FreshShuffleHandler() {
    cntVal = 0;
}

