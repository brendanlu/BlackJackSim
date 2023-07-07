#ifndef STRATIN_H
#define STRATIN_H 

/*   
Our contiguous C array from Numpy memoryviews will rely on this file to be
        accessed at the C++ level correctly.

We use this to perform correct pointer arithmetic on both 1D and 2D C arrays
        using the head of the array passed in at Cython level. 

We use templated strategy input files, so we can hardcode this logic into inline functions.
        This is directly based off the templated csv files. 
*/

// d is dealer upcard value
inline char hrdActionFromPtr(char* head, unsigned int p, unsigned int d) 
{ // there are multiple ways to get the same hard tally p 
        return *(head + (p-4)*10 + (d-2));
}

inline char sftActionFromPtr(char* head, unsigned int p, unsigned int d)
{ // there are multiple ways to get the same soft tally p
        return *(head + (p-13)*10 + (d-2));
}

inline char spltActionFromPtr(char* head, unsigned int p, unsigned int d) 
{ // p is the value the player has double of
        return *(head + (p-2)*10 + (d-2));
}

inline double cntFromPtr(double* head, unsigned int val)
{ // val is nominal value of the card 
        return *(head + (val-2));
}

#endif