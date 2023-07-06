#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"

int main() 
{
    Shoe new_shoe(1, 0.25);
    new_shoe.Display();
    cout << "\n\n";
    new_shoe.Shuffle(10);
    new_shoe.Display();
    cout << "\n\n";

    return 0;
}
