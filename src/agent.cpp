#include "agent.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"
#include "types.hpp"


/*
Nullary constructor, which also serves as a 'reset' method for each new hand.
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


/*
Initialize an Agent from pointers to the data read in from strategy files.
This allows for high level control of the strategy to be adjusted and passed in.
*/
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
    hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), 
    stratInit(true), pnl(0), cntVal(0), 
    nActiveHands(1), hIdx(0),
    BJ_PAYOUT(1.5)
{
    for (unsigned int i=0; i<MAX_N_SPLITS + 1; ++i) {
        hands[i] = HandInfo();
    }
}


/*

*/
void Agent::SetBJPayout(double d) 
{
    BJ_PAYOUT = d;
}


/*
This is called when a card has been specifically dealt to the agent. 
*/
void Agent::DealTargetHandler(const Card &dCard) 
{
    hands[hIdx].nCards += 1;
    hands[hIdx].handVal += dCard.val(); 

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {
        hands[hIdx].nSoftAces += 1;
    }
    
    // revert soft to hard values if needed
    // CURRENTLY ASSUMES YOU CAN HAVE A MIX OF HARD AND SOFT ACES IN ONE HAND
    if (hands[hIdx].handVal > BJVAL && hands[hIdx].nSoftAces > 0) { 
        hands[hIdx].handVal -= 10;
        hands[hIdx].nSoftAces -= 1; 
    }

    // pair tracking via toggling flags
    if (hands[hIdx].nCards == 1) {
        hands[hIdx].first = dCard;
    }
    else if (hands[hIdx].nCards == 2 && hands[hIdx].first.face == dCard.face) {
        hands[hIdx].holdingPair = true;
    }

    // natural blackjack checks
    if (hands[hIdx].handVal == BJVAL && hands[hIdx].nCards == 2) {
        if (hands[hIdx].first.face == 'A') {
            hands[hIdx].natBlackJack = true; 
        }
        else if (hands[hIdx].first.val() == 10) {
            hands[hIdx].natBlackJack = true;
        }
    }
}



/*
Called during every deal process on the table, including one in which the  
DealTargetHandler method may also be called. 

Ensure that there is no duplicate logic between these methods. 
*/
void Agent::DealObserveHandler(const Card &dCard) 
{
    // change internal running count
    cntVal += cntFromPtr(cntPtr, dCard.val()); 
}


/*
Called when the agent must make a decision on what to do; it is given a 
const reference to the dealer, which it can query for any necessary 
information to implement game logic.

At the end, it must result in either a ACTION::HIT or ACTION::STAND being 
returned to the calling state in the simulation engine. 

The Agent can read a much broader range of actions from strategy template files, 
but must implement explicit logic to handle them in this method. 
*/
ACTION Agent::YieldAction(const Dealer &dealerRef) 
{
    char internalAction; 

    // perform action lookups via the configured strategy templates and the 
    // inline lookup functions
    //
    // additional to implementing correct game logic, the first 'if' condition 
    // here is crucial for the memory-safeness the lookup functions
    if (hands[hIdx].handVal >= BJVAL) { 
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

    // process the action
    //
    // control sequences here either do some processing and return an ACTION
    // or recursively call this function again after iterating the active hand
    if (internalAction == 'H') {
        return ACTION::HIT;
    }
    else if (internalAction == 'D') {
        // double the wager and hit
        hands[hIdx].wager *= 2; 
        return ACTION::HIT;
    }
    else if (internalAction == 'P') {
        internalAction = 'S'; 
        
    }
    else if (internalAction == 'R') {
        // half the wager, and make the hand go bust manually
        hands[hIdx].wager /= 2; 
        hands[hIdx].natBlackJack = false; 
        hands[hIdx].handVal = BJVAL + 1;
        internalAction = 'S'; 
    }
    else {
        // additional single char codes can be added as further else-if blocks
        // above this final else block here
        //
        // otherwise, it will default to the behaviour of an 'internal stand'
        internalAction = 'S'; 
    }

    // if we have multiple hands, we continue
    if (hIdx + 1 == nActiveHands) {
        return ACTION::STAND; 
    }
    else {
        hIdx += 1;
        return YieldAction(dealerRef); 
    }
}


/*

*/
void Agent::ClearHandler (const Dealer &dealerRef) 
{
    for (unsigned int i=0; i<nActiveHands; ++i) {
        if (hands[i].handVal > BJVAL) {
            pnl -= hands[i].wager;
        }
        else if (hands[i].natBlackJack && !dealerRef.hInfo.natBlackJack) {
            pnl += BJ_PAYOUT * hands[i].wager;
        }
        else if (!hands[i].natBlackJack && dealerRef.hInfo.natBlackJack) {
            pnl -= hands[i].wager; 
        }
        else if (hands[i].handVal > dealerRef.hInfo.handVal) {
            pnl += hands[i].wager; 
        }
        else if (hands[i].handVal < dealerRef.hInfo.handVal) {
            pnl -= hands[i].wager;
        }
        else {
            ;
        }

        // reset hand information
        hands[i] = HandInfo(); 
    }

    nActiveHands = 1; 
    hIdx = 0; 
}


/*

*/
void Agent::FreshShuffleHandler() 
{
    cntVal = 0;
}

