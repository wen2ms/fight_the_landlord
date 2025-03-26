#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>

#include "robot.h"
#include "userplayer.h"
#include "cards.h"

struct BiddingRecord {
    BiddingRecord() {
        reset();
    }
    
    void reset() {
        player = nullptr;
        points = 0;
        bidding_times = 0;
    }
    
    Player* player;
    int points;
    int bidding_times;
};

class GameControl : public QObject {
    Q_OBJECT
  public:
    enum GameStatus {
        kDealingCard,
        kBiddingCard,
        kPlayingAHand
    };
    
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
    
    void on_bid_lord(Player* player, int points);

  signals:
    void player_status_changed(Player* player, PlayerStatus status);
    
    void notify_bid_lord(Player* player, int points);
    
    void game_status_changed(GameStatus status);
    
  private:
    Robot* left_robot_;
    Robot* right_robot_;
    UserPlayer* user_player_;
    
    Player* current_player_;
    
    Player* pending_player_;
    Cards pending_cards_;
    
    Cards all_cards_;
    
    BiddingRecord bidding_record_;
};

#endif  // GAMECONTROL_H
