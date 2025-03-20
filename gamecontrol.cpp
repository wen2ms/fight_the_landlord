#include "gamecontrol.h"

#include <QRandomGenerator>

GameControl::GameControl(QObject *parent) : QObject{parent} {}

void GameControl::player_init() {
    left_robot_ = new Robot("Robot Left", this);
    right_robot_ = new Robot("Robot Right", this);
    user_player_ = new UserPlayer("Mine", this);
    
    left_robot_->set_direction(Player::Direction::kLeft);
    right_robot_->set_direction(Player::Direction::kRight);
    user_player_->set_direction(Player::Direction::kRight);
    
    Player::Sex sex;
    
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    left_robot_->set_sex(sex);
    
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    right_robot_->set_sex(sex);
    
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    user_player_->set_sex(sex);
    
    user_player_->set_prev_player(left_robot_);
    user_player_->set_next_player(right_robot_);
    
    left_robot_->set_prev_player(right_robot_);
    left_robot_->set_next_player(user_player_);
    
    right_robot_->set_prev_player(user_player_);
    right_robot_->set_next_player(left_robot_);
    
    current_player_ = user_player_;
}

Robot *GameControl::left_robot() {
    return left_robot_;
}

Robot *GameControl::right_robot() {
    return right_robot_;
}

UserPlayer *GameControl::user_player() {
    return user_player_;
}

void GameControl::set_current_player(Player *player) {
    current_player_ = player;
}

Player *GameControl::current_player() {
    return current_player_;
}

Player *GameControl::pending_player() {
    return pending_player_;
}

Cards GameControl::pending_cards() {
    return pending_cards_;
}
