#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include <QPixmap>

#include "card.h"
#include "player.h"

class CardPanel : public QWidget {
    Q_OBJECT
  public:
    explicit CardPanel(QWidget *parent = nullptr);
    
    void set_image(const QPixmap& front, const QPixmap& back);
    QPixmap get_image();
    
    void set_front_side(bool is_front);
    bool is_front_side();
    
    void set_selected_side(bool is_selected);
    bool is_selected();
    
    void set_card(Card& card);
    Card get_card();
    
    void set_owner(Player* player);
    Player* get_owner();
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
    void mousePressEvent(QMouseEvent* event) override;

  signals:
    
  private:
    QPixmap front_;
    QPixmap back_;
    
    bool is_front_;
    bool is_selected_;
    
    Card card_;
    Player* player_;
};

#endif  // CARDPANEL_H
