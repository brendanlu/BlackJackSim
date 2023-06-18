#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"

int main() {

    Shoe new_shoe(2);
    new_shoe.Display();
    cout << "\n\n";
    new_shoe.Shuffle(7);
    new_shoe.Display();

    return 0;
}
