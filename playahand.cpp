#include "playahand.h"

PlayAHand::PlayAHand() {}

PlayAHand::PlayAHand(Cards &cards) {
    classify(cards);
    
    judge_card_type();
}

PlayAHand::PlayAHand(HandType type, Card::CardRank rank, int extra_info) {
    hand_type_ = type;
    card_rank_ = rank;
    extra_info_ = extra_info;   
}

void PlayAHand::classify(Cards &cards) {
    Card::CardList card_list = cards.to_card_list();
    int card_record[Card::CardRank::kRankEnd];
    
    memset(card_record, 0, sizeof(int) * Card::CardRank::kRankEnd);
    
    for (int i = 0; i < card_list.size(); ++i) {
        Card card = card_list.at(i);
        
        card_record[card.rank()]++;
    }
    
    one_card_.clear();
    two_card_.clear();
    three_card_.clear();
    four_card_.clear();
    
    for (int i = 0; i < Card::CardRank::kRankEnd; ++i) {
        if (card_record[i] == 1) {
            one_card_.push_back((Card::CardRank)i);
        } else if (card_record[i] == 2) {
            two_card_.push_back((Card::CardRank)i);
        } else if (card_record[i] == 3) {
            three_card_.push_back((Card::CardRank)i);
        } else if (card_record[i] == 4) {
            four_card_.push_back((Card::CardRank)i);
        }
    }
}

void PlayAHand::judge_card_type() {}

PlayAHand::HandType PlayAHand::hand_type() {
    return hand_type_;
}

Card::CardRank PlayAHand::card_rank() {
    return card_rank_;
}

int PlayAHand::extra_info() {
    return extra_info_;
}
