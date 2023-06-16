#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <array>

#include "simengine.hpp"
#include "deck.hpp"

card_t DECK[DECK_SIZE]; 

auto rng = std::default_random_engine {}; 

// testing function
char bounce_back(int a) {return FACES[0];}
