#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"

class Strategy {
  public:
    Strategy(Player* player, const Cards& cards);
    
    Cards find_same_rank_cards(Card::CardRank rank, int count);
    
    QVector<Cards> find_cards_by_count(int count);
    
    Cards get_range_cards(Card::CardRank begin, Card::CardRank end);
    
  private:
    Player* player_;
    Cards cards_;
};

#endif  // STRATEGY_H
