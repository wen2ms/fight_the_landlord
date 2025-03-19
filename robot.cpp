#include "robot.h"

Robot::Robot(QObject *parent) : Player{parent} {
    type_ = kRobot;
}

void Robot::prepare_bid_lord() {}

void Robot::prepare_play_a_hand() {}
