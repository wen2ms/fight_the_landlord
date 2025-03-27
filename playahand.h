#ifndef PLAYAHAND_H
#define PLAYAHAND_H

#include "cards.h"

class PlayAHand {
  public:
    enum HandType
    {
        kHandUnknown,
        kHandPass,
        
        kHandSingle,
        kHandPair,
        
        kHandTriple,
        kHandTripleSingle,
        kHandTriplePair,
        
        kHandPlane,
        kHandPlaneTwoSingle,
        kHandPlaneTwoPair,
        
        kHandSeqPair,
        kHandSeqSingle,
        
        kHandBomb,
        kHandBombSingle,
        kHandBombPair,
        kHandBombTwoSingle,
        
        kHandBombJokers,
        kHandBombJokersSingle,
        kHandBombJokersPair,
        kHandBombJokersTwoSingle
    };

    
    PlayAHand();
    PlayAHand(Cards& cards);
    PlayAHand(HandType type, Card::CardRank rank, int extra_info);
    
    void classify(Cards& cards);
    
    void judge_card_type();
    
    HandType hand_type();
    
    Card::CardRank card_rank();
    
    int extra_info();
    
  private:
    HandType hand_type_;
    Card::CardRank card_rank_;
    int extra_info_;
    
    QVector<Card::CardRank> one_card_;
    QVector<Card::CardRank> two_card_;
    QVector<Card::CardRank> three_card_;
    QVector<Card::CardRank> four_card_;
};

#endif  // PLAYAHAND_H
