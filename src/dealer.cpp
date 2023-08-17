#include "communication.hpp"
#include "dealer.hpp"
#include "simenginebj.hpp"
#include "types.hpp"

/*
Nullary constructor, which also serves as a 'reset' method
*/
Dealer::HandInfo::HandInfo() : 
    handVal(0), 
    nSoftAces(0), 
    checkBJ(false), 
    natBlackJack(false), 
    upCard(BLANK_CARD) 
{}

/*
All member classes of the simulation engine have explicit nullary constructors 
provided. This is a design decision to minimise issues with the Cython wrappers.
*/
Dealer::Dealer() : 
    hInfo(HandInfo()),
    HITSOFT17(false)
{}

/*

*/
void Dealer::DealTargetHandler(Card dCard) 
{
    hInfo.handVal += dCard.val(); 

    if (dCard.face == 'A') {
        hInfo.nSoftAces += 1;
    }
    
    // revert soft 11's to hard 1's where appropriate
    if (hInfo.handVal > BJVAL && hInfo.nSoftAces > 0) { 
        hInfo.handVal -= 10;
        hInfo.nSoftAces -= 1; 
    }

    // record the first card recieved
    // check the second card for a natural blackjack
    // then turn off flags for subsequent deals
    if (!hInfo.upCard) {
        hInfo.upCard = dCard; 
        hInfo.checkBJ = true;
    }
    else if (hInfo.checkBJ) {
        if (hInfo.handVal == BJVAL) {
            hInfo.natBlackJack = true;
        }
        hInfo.checkBJ = false;
    }
}

/*
Simple dealer logic, with additional Boolean condition dictating whether or not
to hit soft 17's. 
*/
ACTION Dealer::YieldAction() 
{
    if (hInfo.handVal < 17) {
        return ACTION::HIT; 
    }
    else {
        // check for soft 17, and hit if necessary
        if (HITSOFT17 && hInfo.nSoftAces > 0 && hInfo.handVal == 17) {
            return ACTION::HIT; 
        }
        return ACTION::STAND; 
    }
}

/*

*/
void Dealer::ClearHandler() 
{
    hInfo = HandInfo(); 
}