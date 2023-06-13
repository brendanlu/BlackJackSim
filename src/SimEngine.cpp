#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>
using std::unordered_map;

#include "SimEngine.hpp"

const char CARDS[13] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
const char SUITS[4] = {'D', 'S', 'C', 'H'};


auto rng = std::default_random_engine {}; 




int bounce_back(int a) {return a;}
