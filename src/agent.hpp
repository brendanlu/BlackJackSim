#ifndef AGENT_H
#define AGENT_H 

class Agent 
{
public:
    Agent(); // cython needs nullary constructor, and this is memeber class of simenginebj
    Agent(char* hrd, char* sft, char* splt, double* cnt); 

private:
    bool stratInit; // basically keeps track of if the pointers below are actually pointing to strats
    char* hrdPtr; char* sftPtr; char* spltPtr; double* cntPtr;  
};

#endif