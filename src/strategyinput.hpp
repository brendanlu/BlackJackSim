#ifndef STRATIN_H
#define STRATIN_H 

/*   
Functions to retrieve the appropriate actions from the templated strategy input
files. 

THESE ARE NOT MEMORY SAFE, AND CARE MUST BE TAKEN IN THE CALLING STATE TO AVOID 
MISINDEXING. 

Our contiguous C array from Numpy memoryviews will rely on this file to be 
accessed at the C++ level correctly.

We use this to perform correct pointer arithmetic on both 1D and 2D C arrays, 
starting from the head of the array passed in at Cython level. 
*/

// :p: player value
// :d: dealer value

inline char hrdActionFromPtr(char* stratHead, unsigned int p, unsigned int d) 
{
        return *(stratHead + (p-4)*10 + (d-2));
}

inline char sftActionFromPtr(char* stratHead, unsigned int p, unsigned int d)
{
        return *(stratHead + (p-13)*10 + (d-2));
}

inline char spltActionFromPtr(char* stratHead, unsigned int p, unsigned int d) 
{
        return *(stratHead + (p-2)*10 + (d-2));
}

inline double cntFromPtr(double* countHead, unsigned int val)
{
        return *(countHead + (val-2));
}

#endif