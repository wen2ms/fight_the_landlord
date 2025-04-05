#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>
#include <QPixmap>

class AnimationWindow : public QWidget {
    Q_OBJECT
  public:
    explicit AnimationWindow(QWidget *parent = nullptr);
    
    void show_bid_points(int points);
    
  protected:
    void paintEvent(QPaintEvent* event);
    
  signals:
    
  private:
    QPixmap image_;
};

#endif  // ANIMATIONWINDOW_H
