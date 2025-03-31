#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playahand.h"

class Strategy {
  public:
    Strategy(Player* player, const Cards& cards);
    
    Cards find_same_rank_cards(Card::CardRank rank, int count);
    
    QVector<Cards> find_cards_by_count(int count);
    
    Cards get_range_cards(Card::CardRank begin, Card::CardRank end);
    
    QVector<Cards> find_cards_by_type(PlayAHand hand, bool beat);
    
  private:
    QVector<Cards> get_satisfied_cards(Card::CardRank rank_begin, int count);
    
    QVector<Cards> get_triple_single_or_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type);
    
    QVector<Cards> get_plane(Card::CardRank rank_begin);
    
    QVector<Cards> get_plane_two_single_or_two_pair(Card::CardRank rank_begin, PlayAHand::HandType hand_type);
        
    Player* player_;
    Cards cards_;
};

#endif  // STRATEGY_H
