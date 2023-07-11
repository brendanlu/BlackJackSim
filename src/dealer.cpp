#include "dealer.hpp"

// the compiler needs to find a nullary constructor for the nested Dealer struct 
//      otherwise the SimEngine constructor will not work 
Dealer::Dealer() : HITSOFT17(false), handVal(0), nSoftAces(0), upCard(BLANK_CARD) {}

void Dealer::DealTargetHandler(Card dCard) {
    if (dCard.face == 'A') {nSoftAces += 1;}
    handVal += dCard.val(); 
  
    if (handVal > BJVAL && nSoftAces > 0) { // revert soft count to hard count
    handVal -= 10; // adjust ace value to 1 
    nSoftAces -= 1; 
    }

    if (upCard == BLANK_CARD) {upCard == dCard;}
}

void Dealer::ClearHandler() {
    upCard = BLANK_CARD;
}