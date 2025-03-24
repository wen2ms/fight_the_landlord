#include "mainwindow.h"

#include <QPainter>
#include <QRandomGenerator>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    ui->button_group->init_buttons();
    
    ui->button_group->select_panel(ButtonGroup::Panel::kSatrt);
    
    int num = QRandomGenerator::global()->bounded(10);
    QString filepath = QString(":/images/background-%1.png").arg(num + 1);
    background_image_.load(filepath);
    
    this->setWindowTitle("Fight The Landord");
    this->setFixedSize(1080, 720);
    
    game_control_init();
    
    update_scores();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::game_control_init() {
    game_control_ = new GameControl(this);
    
    game_control_->player_init();
    
    Robot* left_robot = game_control_->left_robot();
    Robot* right_robot = game_control_->right_robot();
    UserPlayer* user_player = game_control_->user_player();
    
    player_list_ << left_robot << right_robot << user_player;   
}

void MainWindow::update_scores() {
    ui->score_panel->set_scores(player_list_[0]->score(), player_list_[1]->score(), player_list_[2]->score());
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), background_image_);
}
