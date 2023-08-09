#include "agent.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"


/*
Nullary constructor, which also serves as a 'reset' method
*/
Agent::HandInfo::HandInfo() : 
    wager(0), 
    nCards(0), 
    handVal(0), 
    nSoftAces(0), 
    first(BLANK_CARD), 
    holdingPair(false), 
    natBlackJack(false)
{}


/*

*/
Agent::Agent() : 
    // explicitly flag that we do not have pointers to strats yet
    stratInit(false) 
{}


// init the Agent from pointers to the data read in from strategy files
// these allow high level control of the strategy to be adjusted and passed in
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
    hrdPtr(hrd), 
    sftPtr(sft), 
    spltPtr(splt), 
    cntPtr(cnt), 
    stratInit(true), 
    stackVal(0), 
    cntVal(0), 
    nActiveHands(1), 
    hIdx(0),
    BJ_PAYOUT(1.5)
{
    for (unsigned int i=0; i<MAX_N_SPLITS + 1; ++i) {
        hands[i] = HandInfo();
    }
}

void Agent::SetBJPayout(double d) {
    BJ_PAYOUT = d;
}

// logic for recieving one card into the active hand
void Agent::DealTargetHandler(const Card &dCard) {

    hands[hIdx].nCards += 1;

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {hands[hIdx].nSoftAces += 1;}
    hands[hIdx].handVal += dCard.val(); 
    if (
        hands[hIdx].handVal > BJVAL 
        && 
        hands[hIdx].nSoftAces > 0
        ) 
    { // revert soft count to hard count
        hands[hIdx].handVal -= 10; // adjust ace value to 1 
        hands[hIdx].nSoftAces -= 1; 
    }

    // track if we have pairs
    if (hands[hIdx].nCards == 1) {
        hands[hIdx].first = dCard;
    }
    else if (hands[hIdx].nCards == 2 && hands[hIdx].first.face == dCard.face) {
        hands[hIdx].holdingPair = true;
    }

    // check for instant blackjack
    if (hands[hIdx].handVal == BJVAL && hands[hIdx].nCards == 2) {
        if (hands[hIdx].first.face == 'A') {
            hands[hIdx].natBlackJack = true; 
        }
        else if (hands[hIdx].first.val() == 10) {
            hands[hIdx].natBlackJack = true;
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
ACTION Agent::YieldAction(const Dealer &dealerRef) {
    /*
    One can design any number of actions, restricted by the number of valid unique char codes. 

    The SimEngine will only process 'H' and 'S' codes; the rest of these actions
        one must implement the logic themselves in this agent class here. 
    */

    // note that an internal stand is a stand on the current hand
    // an external stand means the player is done on all hands
    char internalAction; 

    // perform action lookup
    if (hands[hIdx].handVal >= BJVAL) { 
        // we do nothing if we bust; changing this will break game logic - so do with caution
        //      it will also break the lookup via the preconfigured templates, through misindexing problems
        internalAction = 'S'; 
    }
    else if (hands[hIdx].holdingPair) {
        internalAction = spltActionFromPtr(
            spltPtr, 
            hands[hIdx].first.val(), 
            dealerRef.hInfo.upCard.val()
        );
    }
    else if (hands[hIdx].nSoftAces > 0) {
        internalAction = sftActionFromPtr(
            sftPtr, 
            hands[hIdx].handVal, 
            dealerRef.hInfo.upCard.val()
        ); 
    }
    else {
        internalAction = hrdActionFromPtr(
            hrdPtr, 
            hands[hIdx].handVal, 
            dealerRef.hInfo.upCard.val()
        ); 
    }

    // --------------------------------------------------------------------------
    // let us process internal actions
    if (internalAction == 'H') { // we can immediately action a hit command
        return ACTION::HIT;
    }
    // following actions require some processing on the Agent side before we send a message to the simulation engine
    else if (internalAction == 'D') {
        hands[hIdx].wager *= 2; 
        return ACTION::HIT;
    }

    else if (internalAction == 'P') {
        internalAction = 'S'; 
        return ACTION::STAND; // this will need to change
        
    }
    else if (internalAction == 'R') {
        hands[hIdx].wager /= 2; 
        hands[hIdx].natBlackJack = false; 
        hands[hIdx].handVal = BJVAL + 1; // make hand bust
        internalAction = 'S';
        return ACTION::STAND;
    }
    else {
        // we can do some flagging here if we have found some other codes
        // otherwise treat all other codes like a stand
        // internalAction = 'S'; 

        // if we have multiple hands, we continue
        if (hIdx + 1 == nActiveHands) {
            return ACTION::STAND; 
        }
        else {
            hIdx += 1;
            return YieldAction(dealerRef); 
        }
    }   
}

void Agent::ClearHandler (const Dealer &dealerRef) {
    for (unsigned int i=0; i<nActiveHands; ++i) {
        if (hands[i].handVal > BJVAL) {
            stackVal -= hands[i].wager;
        }
        else if (hands[i].natBlackJack && !dealerRef.hInfo.natBlackJack) {
            stackVal += BJ_PAYOUT * hands[i].wager;
        }
        else if (!hands[i].natBlackJack && dealerRef.hInfo.natBlackJack) {
            stackVal -= hands[i].wager; 
        }
        else if (hands[i].handVal > dealerRef.hInfo.handVal) {
            stackVal += hands[i].wager; 
        }
        else if (hands[i].handVal < dealerRef.hInfo.handVal) {
            stackVal -= hands[i].wager;
        }
        else {
            ;
        }

        hands[i] = HandInfo(); // reset hand information
    }


    nActiveHands = 1; 
    hIdx = 0; 
}

void Agent::FreshShuffleHandler() {
    cntVal = 0;
}

