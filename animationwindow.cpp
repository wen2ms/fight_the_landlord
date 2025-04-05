#include "animationwindow.h"

#include <QPainter>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget *parent) : QWidget{parent} {}

void AnimationWindow::show_bid_points(int points) {
    if (points == 1) {
        image_.load(":/images/score1.png");
    } else if (points == 2) {
        image_.load(":/images/score2.png");
    } else if (points == 3) {
        image_.load(":/images/score3.png");
    }
    
    update();
    
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(0, 0, image_.width(), image_.height(), image_);
}
