#include <iostream>
using std::cout;
#include <random>

#include "shoe.hpp"
#include "types.hpp"
#include "simenginebj.hpp"
#include "agent.hpp"

int main() 
{
    
    SimEngineBJ tryEngine(1, 0.5);
    tryEngine.Test();
    cout << "\n\n -------------------------------------- \n\n TEST RAN SUCCESSFULLY IN MAIN\n\n"; 

    cout << "Trying pushback deal \n\n";

    Shoe tryShoe(1, 1); 
    cout << "We can make a dodgy Shoe \n\n"; 


    SimEngineBJ tryEngine2(1, 1); 
    cout << "We can make a dodgy engine \n\n"; 

    cout << "Testing event loops \n\n"; 
    cout << "ENGINE 1\n"; 
    cout << "Status: " << (int)tryEngine.RunSimulation(100) << "\n\n"; 

    cout << "ENGINE 2\n"; 
    cout << "Status: " << (int)tryEngine2.RunSimulation(100) << "\n\n"; 

    return 0;
}
