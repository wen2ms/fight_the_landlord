#include "player.h"

Player::Player(QObject *parent) : QObject{parent} {}

Player::Player(QString name, QObject *parent) {
    name_ = name;
}

void Player::set_name(QString name) {
    name_ = name;
}

QString Player::name() {
    return name_;
}

void Player::set_role(Role role) {
    role_ = role;
}

Player::Role Player::role() {
    return role_;
}

void Player::set_sex(Sex sex) {
    sex_ = sex;
}

Player::Sex Player::sex() {
    return sex_;
}

void Player::set_direction(Direction direction) {
    direction_ = direction;
}

Player::Direction Player::direction() {
    return direction_;
}

void Player::set_type(Type type) {
    type_ = type;
}

Player::Type Player::type() {
    return type_;
}

void Player::set_score(int score) {
    score_ = score;
}

int Player::score() {
    return score_;
}

void Player::set_win(bool flag) {
    is_win_ = flag;
}

bool Player::is_win() {
    return is_win_;
}

void Player::set_prev_player(Player* player) {
    prev_player_ = player;
}

void Player::set_next_player(Player* player) {
    next_player_ = player;
}

Player* Player::prev_player() {
    return prev_player_;
}

Player* Player::next_player() {
    return next_player_;
}

void Player::bid_lord(int point) {}

void Player::store_dealt_card(Card& card) {
    cards_.add(card);
}

void Player::store_dealt_card(Cards& cards) {
    cards_.add(cards);
}

Cards Player::cards() {
    return cards_;
}

void Player::clear_cards() {
    cards_.clear();
}

void Player::play_a_hand(Cards& cards) {
    cards_.remove(cards);
}
