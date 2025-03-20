#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>

#include "robot.h"
#include "userplayer.h"
#include "cards.h"

class GameControl : public QObject {
    Q_OBJECT
  public:
    enum PlayerStatus {
        kPrepareBidLord,
        kPreparePlayAHand,
        kWin
    };
    
    explicit GameControl(QObject *parent = nullptr);
    
    void player_init();
    
    Robot* left_robot();
    Robot* right_robot();
    UserPlayer* user_player();
    
    void set_current_player(Player* player);
    Player* current_player();
    
    Player* pending_player();
    Cards pending_cards();
    
    void init_all_cards();
    
    Card take_one_card();
    Cards take_remaining_cards();
    
    void reset_all_cards();
    
    void start_bid_lord();
    
    void set_lord(Player* player);
    
    void clear_player_score();

  signals:
    
  private:
    Robot* left_robot_;
    Robot* right_robot_;
    UserPlayer* user_player_;
    
    Player* current_player_;
    
    Player* pending_player_;
    Cards pending_cards_;
    
    Cards all_cards_;
};

#endif  // GAMECONTROL_H
