#include "card.h"

Card::Card() {}

void Card::set_suit(CardSuit suit) {
    suit_ = suit;
}

void Card::set_rank(CardRank rank) {
    rank_ = rank;
}

Card::CardSuit Card::suit() const {
    return suit_;
}

Card::CardRank Card::rank() const {
    return rank_;
}
