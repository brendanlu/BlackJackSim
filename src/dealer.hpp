#ifndef DEALER_H
#define DEALER_H

#include "types.hpp"

/*
A simple struct to encapsulate the information and logic of a simulation dealer. 
*/
struct Dealer
{
    struct HandInfo {

        HandInfo();

        unsigned int handVal; 
        unsigned int nSoftAces;

        // check for natural blackjack
        // :checkBJ: will be true ONLY when recieving second card
        // :natBlacJack: flag Dealer has natural BJ 
        bool checkBJ; 
        bool natBlackJack; 

        Card upCard;

    } hInfo;

    Dealer();

    bool HITSOFT17;

    void DealTargetHandler(Card dCard);

    ACTION YieldAction();
    
    void ClearHandler(); 
};

#endif