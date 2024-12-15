#include "../commonTypes.hpp"

class Dealer {
    Dealer();
    ~Dealer() = default;

    public:

    private:
    void initDeck();
    void shuffleDeck();

    const Card *deck;
};