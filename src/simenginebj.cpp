#include "simenginebj.hpp"
#include "strategyinput.hpp"

SimEngineBJ::SimEngineBJ(
        unsigned int ndecks, double penen, 
        char* hrd, char* sft, char* splt, float* cnt
    ) : 
    simShoe(ndecks, penen), hrdPtr(hrd), sftPtr(sft), spltPtr(splt), cntPtr(cnt)
{
    ;
}