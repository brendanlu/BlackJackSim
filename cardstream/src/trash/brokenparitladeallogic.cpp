unsigned int nSuccessShuffled; // how many new shuffled cards successfuly generated

    if (skip == 0) { // just a new partial "reset" shuffle of the shoe
        nDealt = 0;
        nShuffled = FYShuffle(&cardStream[0], N_CARDS, nPartial, mersenneTwister); 
        nDiscarded = 0;
    }
    else { // in this case, we are asking for more shuffled cards; we are still playing the same shoe
        nSuccessShuffled = FYShuffle(&cardStream[skip], N_CARDS - skip, nPartial, mersenneTwister);
        nShuffled += nSuccessShuffled;

        // this is the rare case where our shoe actually runs out
        // in this case, we reshuffle the discarded cards already dealt out
        // we then trigger a flag to reshuffle everything the next time we collect all the cards
        if (nSuccessShuffled < nPartial) {
            nDealt = 0;
            nShuffled = FYShuffle(&cardStream[0], nDiscarded, nPartial - nSuccessShuffled, mersenneTwister);
        }
    }