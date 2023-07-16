#include "dealer.hpp"
#include "types.hpp"

Dealer::HandInfo::HandInfo() : 
        handVal(0), 
        nSoftAces(0), 
        checkBJ(false), 
        natBlackJack(false), 
        upCard(BLANK_CARD) 
{}

// the compiler needs to find a nullary constructor for the nested Dealer struct 
//      otherwise the SimEngine constructor will not work 
Dealer::Dealer() : 
        HITSOFT17(false), 
        hInfo(HandInfo()) 
{}

void Dealer::DealTargetHandler(Card dCard) {
    if (dCard.face == 'A') {hInfo.nSoftAces += 1;}
    hInfo.handVal += dCard.val(); 
  
    if (hInfo.handVal > BJVAL && hInfo.nSoftAces > 0) { // revert soft count to hard count
        hInfo.handVal -= 10; // adjust ace value to 1 
        hInfo.nSoftAces -= 1; 
    }

    if (hInfo.checkBJ) {
        if (hInfo.handVal == BJVAL) {hInfo.natBlackJack = true;}
        hInfo.checkBJ = false;
    }
    else if (!hInfo.upCard) {
        hInfo.upCard = dCard; 
        hInfo.checkBJ = true;
    }
}

ACTION Dealer::YieldAction() {
    if (hInfo.handVal < 17) {
        return ACTION::HIT; 
    }
    else {
        if (HITSOFT17 && hInfo.nSoftAces > 0 && hInfo.handVal == 17) {
            return ACTION::HIT; 
        }
        return ACTION::STAND; 
    }
}

void Dealer::ClearHandler() {
    hInfo = HandInfo(); 
}