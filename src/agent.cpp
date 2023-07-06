#include "types.hpp"
#include "agent.hpp"
#include "strategyinput.hpp"

Agent::Agent() : stratInit(false) {}

Agent::Agent(char* hrd, char* sft, char* splt, double* cnt) : 
hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt), stratInit(true), 
cntVal(0), hInfo({0, 0, 0}) {}

void Agent::dealHandler(Card dCard) {
    cntVal += cntFromPtr(cntPtr, dCard.val()); // change internal running count
    hInfo.hrdVal += dCard.val(); // add to hard count
}