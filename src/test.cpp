#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"
#include "simenginebj.hpp"
#include "agent.hpp"

int main() 
{
    
    SimEngineBJ tryEngine(1, 0.5);
    tryEngine.Test();
    cout << "\n\n -------------------------------------- \n\n TEST RAN SUCCESSFULLY IN MAIN\n\n"; 


    return 0;
}
