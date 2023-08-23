#ifndef LOGGER_H
#define LOGGER_H

#include <array>
#include <iostream>

/*
Simple logging class - easy to wrap in Cython
*/




// Logging mechanism: 
//      [S]: 
//      [A]: 



/*
Various error codes
*/
enum class ERR_CODE : int 
{
    // generic success code
    SUCCESS = 0, 

    // codes prefixed with 1
    // main sim loop related
    NO_AGENT_STRAT = 101,
};


#endif