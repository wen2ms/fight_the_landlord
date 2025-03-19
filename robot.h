#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>

#include "player.h"

class Robot : public Player {
    Q_OBJECT
  public:
    using Player::Player;
    
    explicit Robot(QObject *parent = nullptr);
    
    void prepare_bid_lord() override;
    void prepare_play_a_hand() override;
};

#endif  // ROBOT_H
