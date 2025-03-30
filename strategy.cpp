#include "strategy.h"

Strategy::Strategy(Player *player, const Cards &cards) {
    player_ = player;
    cards_ = cards;
}

Cards Strategy::find_same_rank_cards(Card::CardRank rank, int count) {
    if (count < 1 || count > 4) {
        return Cards();
    }
    
    if (rank == Card::CardRank::kCardSJ || rank == Card::CardRank::kCardBJ) {
        if (count > 1) {
            return Cards();
        }
        
        Card card(Card::CardSuit::kSuitBegin, rank);
        
        if (cards_.contains(card)) {
            Cards cards(card);
            
            return cards;
        } else {
            return Cards();
        }
    }
    
    int find_count = 0;
    Cards find_cards;
    for (int suit = Card::CardSuit::kSuitBegin + 1; suit < Card::CardSuit::kSuitEnd; ++suit) {
        Card card((Card::CardSuit)suit, rank);
        
        if (cards_.contains(card)) {
            find_count++;
            find_cards.add(card);
            
            if (find_count == count) {
                return find_cards;
            }
        }
    }
    
    return Cards();
}
