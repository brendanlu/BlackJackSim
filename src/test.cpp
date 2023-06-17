#include <iostream>
using std::cout;

#include "simengine.hpp"
#include "types.hpp"

int main() {
    cout << "Something OLD\n";

    SimEngine SimEngineInstance(6);

    cout<< "\n\nSomething in test now:\n";
    cout<< SimEngineInstance.NDECKS;




    return 0;
}
