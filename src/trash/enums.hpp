


enum class Suit : char {
    DIAMONDS = 'D', 
    SPADES = 'S', 
    CLUBS = 'C',
    HEARTS = 'H',
};

enum class Face : int { 
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT, 
    NINE, 
    TEN,
    JACK,
    QUEEN,
    KING,
};


typedef struct card {Face face; Suit suit;} card_t;