#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <QMap>
#include <QTimer>

#include "gamecontrol.h"
#include "cardpanel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    void game_control_init();
    
    void update_scores();
    
    void init_cardmap();
    
    void crop_image(QPixmap image, int x, int y, Card& card);
    
    void init_buttons_group();
    
    void init_player_context();
    
    void init_game_scene();
    
    void game_status_process(GameControl::GameStatus status);
    
    void start_dealing_card();
    
    void card_move_step(Player* current_player, int current_card_pos);
    
    void dealt_card_process(Player* player, Cards& cards);
    
    void update_player_cards(Player* player);
    
    void on_deal_card();
    
    void on_player_status_changed(Player* player, GameControl::PlayerStatus status);
    
    void on_bid_lord(Player* player, int points, bool is_first_bidding);
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
  private:
    enum CardAlignment {kHorizontal, kVertical};
    
    struct PlayerContext {
        QRect cards_rect;
        QRect play_a_hand_rect;
        CardAlignment alignment;
        bool is_front_side;
        QLabel* info;
        
        QLabel* role_image;
        Cards last_cards;
    };
    
    Ui::MainWindow *ui;
    
    QPixmap background_image_;
    
    GameControl* game_control_;
    QVector<Player*> player_list_;
    
    QMap<Card, CardPanel*> card_map_;
    QSize card_size_;
    QPixmap card_back_image_;
    
    QMap<Player*, PlayerContext> context_map_;
    
    CardPanel* base_card_;
    CardPanel* moving_card_;
    QVector<CardPanel*> last_three_cards_;
    
    QPoint base_card_pos_;
    
    GameControl::GameStatus game_status_;
    
    QTimer* timer_;
};
#endif  // MAINWINDOW_H
