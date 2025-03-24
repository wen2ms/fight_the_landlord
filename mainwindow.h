#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

#include "gamecontrol.h"

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
    
  protected:
    void paintEvent(QPaintEvent* event) override;
    
  private:
    Ui::MainWindow *ui;
    
    QPixmap background_image_;
    
    GameControl* game_control_;
    QVector<Player*> player_list_;
};
#endif  // MAINWINDOW_H
