#include "card.hpp"
#include "agent.hpp"
#include "communication.hpp"
#include "dealer.hpp"
#include "strategyinput.hpp"

/*
Nullary constructor, which also serves as a 'reset' method for each new hand.
*/
Agent::HandInfo::HandInfo() : 
    wager(0), 
    nCards(0), 
    handVal(0), 
    nSoftAces(0), 
    first(BLANK_CARD),
    second(BLANK_CARD),
    natBlackJack(false),
    holdingPair(false)
{}

/*
Logic for recieving a card into a hand 
*/
void Agent::HandInfo::Recieve(const Card &dCard)
{
    nCards += 1;
    handVal += dCard.val(); 

    // adjust hand value - with soft count logic
    if (dCard.face == 'A') {
        nSoftAces += 1;
    }
    
    // revert soft to hard values if needed
    // CURRENTLY ASSUMES YOU CAN HAVE A MIX OF HARD AND SOFT ACES IN ONE HAND
    if (handVal > BJVAL && nSoftAces > 0) { 
        handVal -= 10;
        nSoftAces -= 1; 
    }

    // pair tracking via toggling flags
    if (nCards == 1) {
        first = dCard;
    }
    else if (nCards == 2 && first.face == dCard.face) {
        holdingPair = true;
        second = dCard;
    }

    // natural blackjack checks
    if (handVal == BJVAL && nCards == 2) {
        if (first.face == 'A') {
            natBlackJack = true; 
        }
        else if (first.val() == 10) {
            natBlackJack = true;
        }
    }
}

/*

*/
Agent::Agent() : 
    // explicitly flag that we do not have pointers to strats yet
    STRAT_INIT(false) 
{}

/*
Initialize an Agent from pointers to the data read in from strategy files.
This allows for high level control of the strategy to be adjusted and passed in.

We initialize the agent to not have any 'active' hands. The stack array will be 
explicitly populated with 'reset'-ed hInfo structs.
*/
Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
    pnl(0),
    STRAT_INIT(true), BJ_INSTANT(false), BJ_PAYOUT(1.5),
    newIdx(1), currIdx(0),
    strat({hrd, sft, splt, cnt}),
    cntVal(0)
{
    for (unsigned int i=0; i<MAX_N_HANDS; ++i) {
        hands[i] = HandInfo();
    }
}

/*
Messy construction of Agent, currently just for testing and wrapper 
compatability
*/
Agent::Agent(StratPackage s) : 
    Agent(s.hrd, s.sft, s.splt, s.cnt)
{}

/*

*/
void Agent::SetLog(Logger *logPtr) 
{
    log = logPtr;

    log->WriteRow(
        LOG_LEVEL::DETAIL,
        LOG_TYPE::AGENT, 
        CONTEXT_STRING_1,
        "Player "  
            + std::to_string(id) 
            + " logging channel initialised"
    );
}

/*
This is called at the end of every round of the hand, to BOTH 1) settle wagers 
from the previous round, abd 2) place wagers for the next round. 

If there is no 'previous' round to be settled (i.e. at the start of a simulation 
run), the hInfo nested structs of both the Dealer (const reference) and the 
agent (*this) should both be at their initial values, which means that none of 
the if, else-if condtions in the first control sequence will be satisfied. 
*/
void Agent::ClearHandler (const Dealer &dealerRef) 
{
    // iterate over each hand, and settle relevant wagers
    // then reset the hand
    for (unsigned int i=0; i<newIdx; ++i) {
        // bust
        if (hands[i].handVal > BJVAL) {
            pnl -= hands[i].wager;
        }
        // natural blackjack payout
        //
        // currently logic requires the dealer to also NOT have a natural BJ,
        // or the BJ_INSTANT boolean flag to be set to true
        else if (hands[i].natBlackJack) {
            if (BJ_INSTANT || !dealerRef.hInfo.natBlackJack) {
                pnl += BJ_PAYOUT * hands[i].wager;
            }
        }
        // dealer has natural blackjack, and player does not
        //
        // note - results in loss, even if player has 21 in some other fashion
        else if (!hands[i].natBlackJack && dealerRef.hInfo.natBlackJack) {
            pnl -= hands[i].wager; 
        }
        // provided player has less than BJVAL, we now perform a direct 
        // comparison of value
        //
        // player hand value is higher, and not bust
        else if (hands[i].handVal > dealerRef.hInfo.handVal) {
            pnl += hands[i].wager; 
        }
        // player hand value is lower, despite not busting
        else if (hands[i].handVal < dealerRef.hInfo.handVal) {
            // check if dealer has bust
            if (dealerRef.hInfo.handVal > BJVAL) {
                pnl += hands[i].wager;
            }
            else {
                pnl -= hands[i].wager;
            }
        }
        // everything else not explicitly caught above results in push behaviour
        else {
            ;
        }

        // reset current hand before iterating onwards to any subseqeunt hands
        hands[i] = HandInfo(); 
    }

    // logic for placing new wagers
    hands[0].wager = 1.0; 

    // reset agent tracking of hands
    //
    // in the first iteration of the simulation, this will also prepare the 
    // Agent appropriately for the first deal-out
    newIdx = 1; 
    currIdx = 0;
}

/*
Called when agent specifically recieves a card
*/
void Agent::DealTargetHandler(const Card &dCard) 
{
    hands[currIdx].Recieve(dCard); 
}

/*
Called during every deal process on the table, including one in which the  
DealTargetHandler method may also be called. 

Ensure that there is no duplicate logic between these methods. 
*/
void Agent::DealObserveHandler(const Card &dCard) 
{
    // change internal running count
    cntVal += cntFromPtr(strat.cnt, dCard.val()); 
}

/*
Called when the agent must make a decision on what to do; it is given a 
const reference of the dealer (all public), which it can query for any necessary 
information to implement game logic.

At the end, it MUST result in either a ACTION::HIT or ACTION::STAND being 
returned to the calling state in the simulation engine. 

The Agent can read a much broader range of actions from strategy template files, 
but must implement explicit logic to handle them in this method, and eventually
returning the ACTION enumeration. 

    Currently, internal actions are: 
        'S' : stand
        'H' : hit
        'D' : double
        'P' : split
        'R' : surrender
*/
ACTION Agent::YieldAction(const Dealer &dealerRef) 
{
    // use for copy overs
    Card temp;

    char internalAction; 

    // perform action lookups via the configured strategy templates and the 
    // inline lookup functions
    //
    // additional to implementing correct game logic, the first 'if' condition 
    // here is crucial for the memory-safeness of the lookup functions
    if (hands[currIdx].handVal >= BJVAL) {
        // we have already bust - do not do anything more
        internalAction = 'S'; 
    }
    else if (hands[currIdx].nCards < 2) {
        // our current hand is instantiated from a split, we need to hit
        // as attempting to use the strategy input functions will result
        // in memory unsafe behaviour
        return ACTION::HIT;
    }
    else if (hands[currIdx].holdingPair) {
        internalAction = spltActionFromPtr(
            strat.splt, 
            hands[currIdx].first.val(), 
            dealerRef.hInfo.upCard.val()
        );
    }
    else if (hands[currIdx].nSoftAces > 0) {
        internalAction = sftActionFromPtr(
            strat.sft, 
            hands[currIdx].handVal, 
            dealerRef.hInfo.upCard.val()
        ); 
    }
    else {
        internalAction = hrdActionFromPtr(
            strat.hrd, 
            hands[currIdx].handVal, 
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
        hands[currIdx].wager *= 2; 
        return ACTION::HIT;
    }
    else if (internalAction == 'P' && newIdx < MAX_N_HANDS) {
        // spawn two hands in place of one at index newIdx of hands
        // construct new hand with correct wager, and one of the pair cards
        //
        // place at the tail of stack array, allowing for potentially a 
        // previously split hand at position currIdx + 1
        hands[newIdx] = HandInfo(); 
        hands[newIdx].wager = hands[currIdx].wager;
        hands[newIdx].Recieve(hands[currIdx].second); 
        
        // overwrite the stack space of the pre-split hand with the second
        // split hand
        //
        // keep the same wager, and take the other pair card
        temp = hands[currIdx].first;
        hands[currIdx] = HandInfo(); 
        hands[currIdx].wager = hands[newIdx].wager; 
        hands[currIdx].Recieve(temp); 

        newIdx += 1; 

        return YieldAction(dealerRef);
    }
    else if (internalAction == 'R') {
        // half the wager, and make the hand go bust manually
        hands[currIdx].wager /= 2; 
        hands[currIdx].natBlackJack = false; 
        hands[currIdx].handVal = BJVAL + 1;
        internalAction = 'S'; 
    }
    else {
        // additional single char codes can be added as further else-if blocks
        // above this final else block here
        //
        // otherwise, it will default to the behaviour of an 'internal stand'
        internalAction = 'S'; 
    }

    // if we have multiple hands, we continue - ensure this is memory safe
    //
    // else - exit the function and return an action enumeration to the engine
    if (currIdx == newIdx - 1) {
        return ACTION::STAND; 
    }
    else {
        currIdx += 1;
        return YieldAction(dealerRef); 
    }
}

/*

*/
void Agent::FreshShuffleHandler() 
{
    cntVal = 0;
}

