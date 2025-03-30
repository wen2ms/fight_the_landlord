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

QVector<Cards> Strategy::find_cards_by_count(int count) {
    if (count < 1 || count > 4) {
        return QVector<Cards>();
    }
    
    
    QVector<Cards> cards_list;
    for (int rank = Card::CardRank::kCard3; rank < Card::CardRank::kRankEnd; ++rank) {
        if (cards_.rank_count((Card::CardRank)rank) == count) {
            Cards cards;
            
            cards << find_same_rank_cards((Card::CardRank)rank, count);
            
            cards_list << cards;
        }
    }
    
    return cards_list;
}

Cards Strategy::get_range_cards(Card::CardRank begin, Card::CardRank end) {
    Cards range_cards;
    
    for (int rank = begin; rank < end; ++rank) {
        int count = cards_.rank_count((Card::CardRank)rank);
        
        Cards cards = find_same_rank_cards((Card::CardRank)rank, count);
        
        range_cards << cards;
    }
    
    return range_cards;
}
