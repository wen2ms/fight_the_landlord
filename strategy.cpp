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
            return get_triple_single_or_pair(begin_rank, PlayAHand::HandType::kHandSingle);
        case PlayAHand::HandType::kHandTriplePair:
            return get_triple_single_or_pair(begin_rank, PlayAHand::HandType::kHandPair);
        case PlayAHand::HandType::kHandPlane:
            return get_plane(begin_rank);
        case PlayAHand::HandType::kHandPlaneTwoSingle:
            return get_plane_two_single_or_two_pair(begin_rank, PlayAHand::HandType::kHandSingle);
        case PlayAHand::HandType::kHandPlaneTwoPair:
            return get_plane_two_single_or_two_pair(begin_rank, PlayAHand::HandType::kHandPair);
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

QVector<Cards> Strategy::get_triple_single_or_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type) {
    QVector<Cards> find_cards_list = get_satisfied_cards(rank_begin, 3);
    
    if (!find_cards_list.empty()) {
        Cards remain_cards = cards_;
        
        remain_cards.remove(find_cards_list);
        
        Strategy strategy(player_, remain_cards);
        QVector<Cards> cards_list = strategy.find_cards_by_type(PlayAHand(hand_type, Card::CardRank::kRankBegin, 0),
                                                                false);
        
        if (!cards_list.empty()) {
            for (int i = 0; i < find_cards_list.size(); ++i) {
                find_cards_list[i].add(cards_list[0]);
            }
        } else {
            find_cards_list.clear();
        }
    }
    
    return find_cards_list;
}

QVector<Cards> Strategy::get_plane(Card::CardRank rank_begin) {
    QVector<Cards> find_cards_list;
    
    for (int rank = rank_begin; rank <= Card::CardRank::kCardK; ++rank) {
        Cards prev_cards = find_same_rank_cards((Card::CardRank)rank, 3);
        Cards next_cards = find_same_rank_cards((Card::CardRank)(rank + 1), 3);
        
        if (!prev_cards.is_empty() && !next_cards.is_empty()) {
            Cards cards;
            
            cards << prev_cards << next_cards;
            find_cards_list << cards;
        }
    }
    
    return find_cards_list;
}

QVector<Cards> Strategy::get_plane_two_single_or_two_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type) {
    QVector<Cards> find_cards_list = get_plane(rank_begin);
    
    if (!find_cards_list.empty()) {
        Cards remain_cards = cards_;
        
        remain_cards.remove(find_cards_list);
        
        Strategy strategy(player_, remain_cards);
        QVector<Cards> cards_list = strategy.find_cards_by_type(PlayAHand(hand_type, Card::CardRank::kRankBegin, 0),
                                                                false);
        
        if (cards_list.size() >= 2) {
            for (int i = 0; i < find_cards_list.size(); ++i) {
                Cards cards;
                
                cards << cards_list[0] << cards_list[1];
                
                find_cards_list[i].add(cards);
            }
        } else {
            find_cards_list.clear();
        }
    }
    
    return find_cards_list;
}
