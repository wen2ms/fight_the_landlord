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

bool less_card(const Card& card1, const Card& card2) {
    if (card1.rank() == card2.rank()) {
        return card1.suit() < card2.suit();
    } else {
        return card1.rank() < card2.rank();
    }
}

bool greater_card(const Card& card1, const Card& card2) {
    if (card1.rank() == card2.rank()) {
        return card1.suit() > card2.suit();
    } else {
        return card1.rank() > card2.rank();
    }
}
