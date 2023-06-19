#ifndef AGENT_H
#define AGENT_H 

class Agent 
{
public:
    Agent() {};
    Agent(char* hrd, char* sft, char* splt, double* cnt);

    char* hrdPtr; 

private:
    char* sftPtr; char* spltPtr; double* cntPtr;
};

#endif