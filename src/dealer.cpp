#include "dealer.hpp"
#include "types.hpp"

// the compiler needs to find a nullary constructor for the nested Dealer struct 
//      otherwise the SimEngine constructor will not work 
Dealer::Dealer() : HITSOFT17(false), upCard(BLANK_CARD), handVal(0), nSoftAces(0) {}

void Dealer::DealTargetHandler(Card dCard) {
    if (dCard.face == 'A') {nSoftAces += 1;}
    handVal += dCard.val(); 
  
    if (handVal > BJVAL && nSoftAces > 0) { // revert soft count to hard count
    handVal -= 10; // adjust ace value to 1 
    nSoftAces -= 1; 
    }

    if (!upCard) {upCard = dCard;}
}

ACTION Dealer::YieldAction() {
    if (handVal < 17) {
        return ACTION::HIT; 
    }
    else {
        if (HITSOFT17 && nSoftAces > 0) {
            return ACTION::HIT; 
        }
        return ACTION::STAND; 
    }
}

void Dealer::ClearHandler() {
    upCard = BLANK_CARD;

    handVal = 0;
    nSoftAces = 0;
}