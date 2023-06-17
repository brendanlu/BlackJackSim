#ifndef TYPES_H
#define TYPES_H 
#include <iostream> // we need this to pass char into std::ostream

struct card {char face; char suit;};
inline std::ostream& operator<< (std::ostream& os, card const& c) {os<<c.face; os<<c.suit; return os;}
typedef struct card card_t;

#endif