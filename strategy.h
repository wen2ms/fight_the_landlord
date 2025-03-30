#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"

class Strategy {
  public:
    Strategy(Player* player, const Cards& cards);
    
    Cards find_same_rank_cards(Card::CardRank rank, int count);
    
  private:
    Player* player_;
    Cards cards_;
};

#endif  // STRATEGY_H
