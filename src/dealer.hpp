#ifndef DEALER_H
#define DEALER_H

/*
A simple struct to encapsulate the information and logic of a simulation dealer. 
*/

#include "types.hpp"

struct Dealer
{
    struct HandInfo {

        HandInfo();

        unsigned int handVal; 
        unsigned int nSoftAces;

        bool checkBJ; // switch on when recieving second card
        bool natBlackJack; // check for 2 card natural blackjack

        Card upCard; // dealer up-card

    } hInfo;

    Dealer();

    bool HITSOFT17;     

    void DealTargetHandler(Card dCard);

    ACTION YieldAction();
    
    void ClearHandler(); 

};

#endif