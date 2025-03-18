#ifndef CARD_H
#define CARD_H

class Card {
  public:
    enum CardSuit {
        kSuitBegin,
        kHeart,
        kDiamond,
        kClub,
        kSpade,
        kSuitEnd
    };
    
    enum CardRank {
        kPointBegin,
        kCard3,
        kCard4,
        kCard5,
        kCard6,
        kCard7,
        kCard8,
        kCard9,
        kCard10,
        kCardJ,
        kCardQ,
        kCardK,
        kCardA,
        kCard2,
        kCardSJ,
        kCardBJ,
        kPointEnd
    };
    
    Card();
    
    void set_suit(CardSuit suit);
    void set_rank(CardRank rank);
    CardSuit suit();
    CardRank rank();
    
  private:
    CardSuit suit_;
    CardRank rank_;
};

#endif  // CARD_H
