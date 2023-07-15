#include "agent.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"

// nullary, 'reset' constructor of the hand information struct
Agent::HandInfo::HandInfo() : nHolding(0), handVal(0), nSoftAces(0), firstCard(BLANK_CARD), holdingPair(false), natBlackJack(false)
{}

Agent::Agent() : stratInit(false) // flag that we do not have pointers to strats yet
{} 

// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), stackVal(0), cntVal(0), nActiveHands(1), activeHandIdx(0)
{
    for (unsigned int i=0; i<MAX_N_SPLITS + 1; ++i) {
        hands[i] = HandInfo();
    }
}

// logic for recieving one card into the active hand
void Agent::DealTargetHandler(const Card &dCard) {

    hands[activeHandIdx].nHolding += 1;

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {hands[activeHandIdx].nSoftAces += 1;}
    hands[activeHandIdx].handVal += dCard.val(); 
    if (hands[activeHandIdx].handVal > BJVAL && hands[activeHandIdx].nSoftAces > 0) { // revert soft count to hard count
        hands[activeHandIdx].handVal -= 10; // adjust ace value to 1 
        hands[activeHandIdx].nSoftAces -= 1; 
    }

    // track if we have pairs
    if (hands[activeHandIdx].nHolding == 1) {hands[activeHandIdx].firstCard = dCard;}
    else if (hands[activeHandIdx].nHolding == 2 && hands[activeHandIdx].firstCard.face == dCard.face) {hands[activeHandIdx].holdingPair = true;}

    // check for instant blackjack
    if (hands[activeHandIdx].handVal == BJVAL && hands[activeHandIdx].nHolding == 2) {
        if (hands[activeHandIdx].firstCard.face == 'A') {
            hands[activeHandIdx].natBlackJack = true; 
        }
        else if (hands[activeHandIdx].firstCard.val() == 10) {
            hands[activeHandIdx].natBlackJack = true;
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
    if (hands[activeHandIdx].handVal >= BJVAL) { 
        // we do nothing if we bust; changing this will break game logic - so do with caution
        //      it will also break the lookup via the preconfigured templates, through misindexing problems
        return 'S'; 
    }
    else if (hands[activeHandIdx].holdingPair) {
        internalAction = spltActionFromPtr(spltPtr, hands[activeHandIdx].firstCard.val(), dealerRef.hInfo.upCard.val());
    }
    else if (hands[activeHandIdx].nSoftAces > 0) {
        internalAction = sftActionFromPtr(sftPtr, hands[activeHandIdx].handVal, dealerRef.hInfo.upCard.val()); 
    }
    else {
        internalAction = hrdActionFromPtr(hrdPtr, hands[activeHandIdx].handVal, dealerRef.hInfo.upCard.val()); 
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
    for (unsigned int i=0; i<nActiveHands; ++i) {
        hands[i] = HandInfo(); // reset hand information
    }
    nActiveHands = 1; 
    activeHandIdx = 0; 
}

void Agent::FreshShuffleHandler() {
    cntVal = 0;
}

