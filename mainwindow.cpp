#include "mainwindow.h"

#include <QPainter>
#include <QRandomGenerator>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    init_buttons_group();
    
    int num = QRandomGenerator::global()->bounded(10);
    QString filepath = QString(":/images/background-%1.png").arg(num + 1);
    background_image_.load(filepath);
    
    this->setWindowTitle("Fight The Landord");
    this->setFixedSize(1080, 720);
    
    game_control_init();
    
    update_scores();
    
    init_cardmap();
    
    init_player_context();
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

void MainWindow::init_cardmap() {
    QPixmap cards_image(":/images/card.png");
    
    card_size_.setWidth(cards_image.width() / 13);
    card_size_.setHeight(cards_image.height() / 5);
    
    card_back_image_ = cards_image.copy(2 * card_size_.width(), 4 * card_size_.height(),
                                        card_size_.width(), card_size_.height());
    
    for (int i = 0, suit = Card::CardSuit::kSuitBegin + 1; suit < Card::CardSuit::kSuitEnd; ++i, ++suit) {
        for (int j = 0, rank = Card::CardRank::kRankBegin + 1; rank < Card::CardRank::kCardSJ; ++j, ++rank) {
            Card card((Card::CardSuit)suit, (Card::CardRank)rank);
            
            crop_image(cards_image, card_size_.width() * i, card_size_.height() * j, card);
        }
    }
    
    Card sj_card(Card::CardSuit::kSuitBegin, Card::CardRank::kCardSJ);
    crop_image(cards_image, 0, 4 * card_size_.height(), sj_card);
    
    Card bj_card(Card::CardSuit::kSuitBegin, Card::CardRank::kCardBJ);
    crop_image(cards_image, card_size_.width(), 4 * card_size_.height(), bj_card);
}

void MainWindow::crop_image(QPixmap image, int x, int y, Card& card) {
    QPixmap sub_image = image.copy(x, y, card_size_.width(), card_size_.height());
    
    CardPanel* card_panel = new CardPanel(this);
    
    card_panel->set_image(sub_image, card_back_image_);
    card_panel->set_card(card);
    card_panel->hide();
    
    card_map_.insert(card, card_panel);
}

void MainWindow::init_buttons_group() {
    ui->button_group->init_buttons();
    
    ui->button_group->select_panel(ButtonGroup::Panel::kSatrt);
    
    connect(ui->button_group, &ButtonGroup::start_game, this, [=]() {});
    connect(ui->button_group, &ButtonGroup::play_a_hand, this, [=]() {});
    connect(ui->button_group, &ButtonGroup::pass, this, [=]() {});
    connect(ui->button_group, &ButtonGroup::bid_points, this, [=]() {});
}

void MainWindow::init_player_context() {
    QRect cards_rect[] = {
        QRect(90, 130, 100, height() - 200),
        QRect(rect().right() - 190, 130, 100, height() - 200),
        QRect(250, rect().bottom() - 120, width() - 500, 100)
    };
    
    QRect play_a_hand_rect[] = {
        QRect(260, 150, 100, 100),
        QRect(rect().right() - 360, 150, 100, 100),
        QRect(150, rect().bottom() - 290, width() - 300, 100)
    };
    
    QPoint role_image_pos[] = {
        QPoint(cards_rect[0].left() - 80, cards_rect[0].height() / 2 + 20),
        QPoint(cards_rect[1].right() + 10, cards_rect[1].height() / 2 + 20),
        QPoint(cards_rect[2].right() - 10, cards_rect[0].top() - 10)
    };
    
    int user_player_index = player_list_.indexOf(game_control_->user_player());
    
    for (int i = 0; i < player_list_.size(); ++i) {
        PlayerContext context;
        
        context.alignment = (i == user_player_index ? CardAlignment::kHorizontal : CardAlignment::kVertical);
        
        context.is_front_side = (i == user_player_index);
        
        context.cards_rect = cards_rect[i];
        
        context.play_a_hand_rect = play_a_hand_rect[i];
        
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide();
        
        QRect rect = play_a_hand_rect[i];
        QPoint info_pos(rect.left() + (rect.width() - context.info->width()) / 2,
                        rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(info_pos);
        
        context.role_image = new QLabel(this);
        context.role_image->resize(84, 120);
        context.role_image->hide();
        context.role_image->move(role_image_pos[i]);
        
        context_map_.insert(player_list_.at(i), context);
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), background_image_);
}
