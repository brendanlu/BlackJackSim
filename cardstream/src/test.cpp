#include <chrono>
#include <iostream>
#include <random>

#include "agent.hpp"
#include "shoe.hpp"
#include "simenginebj.hpp"
#include "card.hpp"

using std::cout;

char hrd[17][10] = {
    {'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'},
    {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H'},
    {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D'},
    {'H', 'H', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'},
    {'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'},
    {'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'H', 'H'},
    {'S', 'S', 'S', 'S', 'S', 'H', 'H', 'H', 'R', 'R'},
    {'S', 'S', 'S', 'S', 'S', 'H', 'H', 'R', 'R', 'R'},
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'R'},
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'},
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'},
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'}
};

char sft[8][10] = {
    {'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'H', 'D', 'D', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'},
    {'H', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H', 'H'},
    {'D', 'D', 'D', 'D', 'D', 'S', 'S', 'H', 'H', 'H'},
    {'S', 'S', 'S', 'S', 'D', 'S', 'S', 'S', 'S', 'S'},
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'}
};

char splt[10][10] = {
    {'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H'},
    {'H', 'H', 'H', 'P', 'P', 'H', 'H', 'H', 'H', 'H'},
    {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'H', 'H'},
    {'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H', 'H'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'H', 'H', 'H', 'H'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'}
};

double cnt[10] = {1,1,1,1,1,0,0,0,-1,-1}; 

unsigned long long NITERS = 100000;

int main() 
{
    cout << "Creating cpp test simengine\n\n"; 
    SimEngineBJ testEngine(6, 0.5);

    cout << "Setting agent strat pointers\n"; 
    testEngine.SetAgent(0, &hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0]);
    testEngine.SetAgent(1, &hrd[0][0], &sft[0][0], &splt[0][0], &cnt[0]);
    cout << "Agents set\n\n";

    cout << "Confifuring logfile\n"; 
    testEngine.SetLogLevel(3); 
    cout << "Set log level success\n"; 
    testEngine.SetLogFile("LOG2.csv");
    cout << "Configured log file.\n";
    testEngine.SetSocketConnection("127.0.0.1", 11111);
    cout << "Socket connection initlsed\n\n";

    cout << "Running simulation of " << NITERS << " iterations\n";
    auto start = std::chrono::system_clock::now();
    testEngine.RunSimulation(NITERS);  
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - start
    );
    cout << "Simulation time: " << elapsed.count() << "ms\n\n"; 
    
    /*
    cout << "Shoe reshuffle iterations: " << testEngine.shoeRounds << "\n\n";

    cout << "Max hands in a shoe: " << testEngine.nPlayed << "\n\n";

    cout << "Total hands played: " << testEngine.total << "\n\n";
    */

    return 0;
}
