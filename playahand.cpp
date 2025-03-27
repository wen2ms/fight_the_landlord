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

void PlayAHand::classify(Cards &cards) {}

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
