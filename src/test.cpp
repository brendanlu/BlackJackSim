#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"
#include "simenginebj.hpp"
#include "agent.hpp"

int main() 
{
    
    cout << "Program start \n"; 
    Shoe new_shoe(1, 0.25);
    new_shoe.Display();
    cout << "\n\n";
    new_shoe.FreshShuffleN(10);
    new_shoe.Display();
    cout << "\n\n";

    Card tryCard = {'3', 'D'};
    cout << tryCard.val() << "\n\n";

    SimEngineBJ tryEngine(2, 0.25); 

    Agent simAgent;

    cout << "Init stuff works \n\n"; 
    // new_shoe.Deal(simDealer);
    new_shoe.Deal(simAgent);
    new_shoe.Deal(simAgent);

    cout << "Agent deal is not broken yet \n\n";

    


    return 0;
}
