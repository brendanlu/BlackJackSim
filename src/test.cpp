#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"

int main() 
{
    cout << "Program start \n"; 
    Shoe new_shoe(1, 0.25);
    new_shoe.Display();
    cout << "\n\n";
    new_shoe.EfficientShuffle(49, 1);
    new_shoe.Display();
    cout << "\n\n";

    Card tryCard = {'3', 'D'};
    cout << tryCard.val();

    return 0;
}
