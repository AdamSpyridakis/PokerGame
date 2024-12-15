enum Suit {
    Diamonds,
    Hearts,
    Clubs,
    Spades
};

enum Value {
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

struct Card {
    const Suit suit;
    const Value value;
};