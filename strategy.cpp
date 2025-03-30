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

QVector<Cards> Strategy::find_cards_by_type(PlayAHand hand, bool beat) {
    PlayAHand::HandType hand_type = hand.hand_type();
    Card::CardRank card_rank = hand.card_rank();
    int extra_info = hand.extra_info();
    
    Card::CardRank begin_rank = beat ? (Card::CardRank)(card_rank + 1) : Card::CardRank::kCard3;
    
    switch (hand_type) {
        case PlayAHand::HandType::kHandSingle:
            return get_satisfied_cards(begin_rank, 1);
        case PlayAHand::HandType::kHandPair:
            return get_satisfied_cards(begin_rank, 2);
        case PlayAHand::HandType::kHandTriple:
            return get_satisfied_cards(begin_rank, 3);
        case PlayAHand::HandType::kHandTripleSingle:
            break;
        case PlayAHand::HandType::kHandTriplePair:
            break;
        case PlayAHand::HandType::kHandPlane:
            break;
        case PlayAHand::HandType::kHandPlaneTwoSingle:
            break;
        case PlayAHand::HandType::kHandPlaneTwoPair:
            break;
        case PlayAHand::HandType::kHandSeqPair:
            break;
        case PlayAHand::HandType::kHandSeqSingle:
            break;
        case PlayAHand::HandType::kHandBomb:
            break;
        default:
            break;
    }
}

QVector<Cards> Strategy::get_satisfied_cards(Card::CardRank rank_begin, int count) {
    QVector<Cards> find_cards_list;
    
    for (int rank = rank_begin; rank < Card::CardRank::kRankEnd; ++rank) {
        Cards cards = find_same_rank_cards((Card::CardRank)rank, count);
        
        if (!cards.is_empty()) {
            find_cards_list << cards;   
        }
    }
    
    return find_cards_list;
}
