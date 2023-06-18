#include <iostream>
using std::cout;

#include "shoe.hpp"
#include "types.hpp"

int main() {
    cout << "Something OLD\n";

    Shoe ShoeInstance(6);

    cout<< "\n\nSomething in test now:\n";
    cout<< "playing with: " << ShoeInstance.NDECKS << " decks\n";

    ShoeInstance.Shuffle();

    return 0;
}
