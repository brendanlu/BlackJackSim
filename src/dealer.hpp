#ifndef DEALER_H
#define DEALER_H

/*
A simple struct to encapsulate the information and logic of a simulation dealer. 
*/

#include "types.hpp"

struct Dealer
{
    Dealer();

    bool HITSOFT17; 

    Card upCard; // dealer up-card

    unsigned int handVal; 
    unsigned int nSoftAces;

    void DealTargetHandler(Card dCard);

    ACTION YieldAction();
    
    void ClearHandler(); 

};

#endif