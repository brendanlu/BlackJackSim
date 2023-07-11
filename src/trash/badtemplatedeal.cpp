

template<typename targetType> void Shoe::Deal(targetType &target) 
{
    /*
    Simulates delaing a card to a player (Agent type).
    Does some simple housekeeping on the Shoe side to make sure deal is ok.
    Then calls the DealTargetHandler method of the target Agent object.

    Returns the success status of this deal. 
    */

    if (nDealt + 1 <= nValidShuffled) { // we have "fresh" shuffled cards to deal
        target.DealTargetHandler(cardStream[nDealt++]);
        if (nDealt > N_UNTIL_CUT) {needReshuffle = true;}
    }
    else { // we can just get some more "fresh" shuffled cards in the cardstream
        PushBackShuffle(); 
        Deal(target);
    }
}

// we deal to Agent classes, and the struct in the simengine representing a dealer
template void Shoe::Deal<Agent>(Agent&); 
template void Shoe::Deal<SimEngineBJ::Dealer>(SimEngineBJ::Dealer&); 