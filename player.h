#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include "cards.h"

class Player : public QObject {
    Q_OBJECT
  public:
    enum Role {kLord, kFarmer};
    enum Sex {kMale, kFemale};
    enum Direction {kLeft, kRight};
    enum Type {kRobot, kUser, kUnknown};
    
    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject* parent = nullptr);
    
    void set_name(QString name);
    QString name();
    
    void set_role(Role role);
    Role role();
    
    void set_sex(Sex sex);
    Sex sex();
    
    void set_direction(Direction direction);
    Direction direction();
    
    void set_type(Type type);
    Type type();
    
    void set_score(int score);
    int score();
    
    void set_win(bool flag);
    bool is_win();
    
    void set_prev_player(Player* player);
    void set_next_player(Player* player);
    
    Player* prev_player();
    Player* next_player();
    
    void bid_lord(int points);
    
    void store_dealt_card(Card& card);
    void store_dealt_card(Cards& cards);
    
    Cards cards();
    
    void clear_cards();
    
    void play_a_hand(Cards& cards);
    
    void set_pending_info(Player* player, Cards& cards);
    Cards pending_cards();
    Player* pending_player();
    
    virtual void prepare_bid_lord();
    virtual void prepare_play_a_hand();
    
  signals:
    void notify_bid_lord(Player* player, int points);
    
    void notify_play_a_hand(Player* player, Cards cards);
    
  protected:
    int score_;
    QString name_;
    Role role_;
    Sex sex_;
    Direction direction_;
    Type type_;
    bool is_win_;
    
    Player* prev_player_;
    Player* next_player_;
    
    Cards cards_;
    
    Cards pending_cards_;
    Player* pending_player_;
};

#endif  // PLAYER_H
