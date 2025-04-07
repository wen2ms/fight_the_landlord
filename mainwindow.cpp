#include "mainwindow.h"

#include <QPainter>
#include <QRandomGenerator>

#include "./ui_mainwindow.h"
#include "playahand.h"

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
    
    init_game_scene();
    
    timer_ = new QTimer(this);
    
    connect(timer_, &QTimer::timeout, this, &MainWindow::on_deal_card);
    
    animation_window_ = new AnimationWindow(this);
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
    
    connect(game_control_, &GameControl::player_status_changed, this, &MainWindow::on_player_status_changed);
    connect(game_control_, &GameControl::notify_bid_lord, this, &MainWindow::on_bid_lord);
    connect(game_control_, &GameControl::game_status_changed, this, &MainWindow::game_status_process);
    connect(game_control_, &GameControl::notify_play_a_hand, this, &MainWindow::on_play_a_hand);
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
            
            crop_image(cards_image, card_size_.width() * j, card_size_.height() * i, card);
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
    
    connect(ui->button_group, &ButtonGroup::start_game, this, [=]() {
        ui->button_group->select_panel(ButtonGroup::Panel::kEmpty);
        game_control_->clear_player_score();
        update_scores();
        
        game_status_process(GameControl::GameStatus::kDealingCard);
    });
    connect(ui->button_group, &ButtonGroup::play_a_hand, this, [=]() {});
    connect(ui->button_group, &ButtonGroup::pass, this, [=]() {});
    connect(ui->button_group, &ButtonGroup::bid_points, this, [=](int points) {
        game_control_->user_player()->bid_lord(points);
        ui->button_group->select_panel(ButtonGroup::Panel::kEmpty);
    });
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

void MainWindow::init_game_scene() {
    base_card_ = new CardPanel(this);
    base_card_->set_image(card_back_image_, card_back_image_);
    
    moving_card_ = new CardPanel(this);
    moving_card_->set_image(card_back_image_, card_back_image_);
    
    for (int i = 0; i < 3; ++i) {
        CardPanel* panel = new CardPanel(this);
        
        panel->set_image(card_back_image_, card_back_image_);
        
        last_three_cards_.push_back(panel);
        
        panel->hide();
    }
    
    base_card_pos_ = QPoint((width() - card_size_.width()) / 2, height() / 2 - 100);
    
    base_card_->move(base_card_pos_);
    moving_card_->move(base_card_pos_);
    
    int last_three_card_base = (width() - 3 * card_size_.width() - 2 * 10) / 2;
    for (int i = 0; i < 3; ++i) {
        last_three_cards_[i]->move(last_three_card_base + (card_size_.width() + 10) * i, 20);
    }
}

void MainWindow::game_status_process(GameControl::GameStatus status) {
    game_status_ = status;

    switch (status) {
        case GameControl::GameStatus::kDealingCard:
            start_dealing_card();
            break;
        case GameControl::GameStatus::kBiddingCard: {
            Card::CardList last_three_cards = game_control_->take_remaining_cards().to_card_list();
            
            for (int i = 0; i < last_three_cards.size(); ++i) {
                QPixmap front = card_map_[last_three_cards.at(i)]->get_image();
                last_three_cards_[i]->set_image(front, background_image_);
                
                last_three_cards_[i]->hide();
            }
            
            game_control_->start_bid_lord();
            break;
        }
        case GameControl::GameStatus::kPlayingAHand:
            break;
        default:
            break;
    }
}

void MainWindow::start_dealing_card() {
    for (auto it = card_map_.begin(); it != card_map_.end(); ++it) {
        it.value()->set_selected_side(false);
        it.value()->set_front_side(true);
        it.value()->hide();
    }
    
    for (int i = 0; i < last_three_cards_.size(); ++i) {
        last_three_cards_.at(i)->hide();
    }
    
    int user_player_index = player_list_.indexOf(game_control_->user_player());
    for (int i = 0; i < player_list_.size(); ++i) {
        context_map_[player_list_.at(i)].last_cards.clear();
        context_map_[player_list_.at(i)].info->hide();
        context_map_[player_list_.at(i)].role_image->hide();
        
        context_map_[player_list_.at(i)].is_front_side = (i == user_player_index);
    }
    
    game_control_->reset_all_cards();
    
    base_card_->show();
    
    ui->button_group->select_panel(ButtonGroup::Panel::kEmpty);
    
    timer_->start(10);
}

void MainWindow::card_move_step(Player* current_player, int current_card_pos) {
    QRect card_rect = context_map_[current_player].cards_rect;
    
    int steps[] = {
        (base_card_pos_.x() - card_rect.right()) / 100,
        (card_rect.left() - base_card_pos_.x()) / 100,
        (card_rect.top() - base_card_pos_.y()) / 100,
    };
    
    QPoint pos[] = {
        QPoint(base_card_pos_.x() - current_card_pos * steps[0], base_card_pos_.y()),
        QPoint(base_card_pos_.x() + current_card_pos * steps[1], base_card_pos_.y()),
        QPoint(base_card_pos_.x(), base_card_pos_.y() + current_card_pos * steps[2])
    };
    
    int current_player_index = player_list_.indexOf(current_player);
    moving_card_->move(pos[current_player_index]);
    
    if (current_card_pos == 0) {
        moving_card_->show();
    }
    
    if (current_card_pos == 100) {
        moving_card_->hide();
    }
}

void MainWindow::dealt_card_process(Player* player, Cards& cards) {
    Card::CardList cards_list = cards.to_card_list();
    for (int i = 0; i < cards_list.size(); ++i) {
        CardPanel* panel = card_map_[cards_list.at(i)];
        
        panel->set_owner(player);
    }
    
    update_player_cards(player);
}

void MainWindow::update_player_cards(Player* player) {
    Cards cards = player->cards();
    Card::CardList cards_list = cards.to_card_list();
    
    int card_space = 20;
    QRect cards_rect = context_map_[player].cards_rect;
    
    for (int i = 0; i < cards_list.size(); ++i) {
        CardPanel* panel = card_map_[cards_list.at(i)];
        
        panel->show();
        panel->raise();
        panel->set_front_side(context_map_[player].is_front_side);
        
        if (context_map_[player].alignment == CardAlignment::kHorizontal) {
            int left = cards_rect.left() + (cards_rect.width() - (cards_list.size() - 1) * card_space - card_size_.width()) / 2;
            int top = cards_rect.top() + (cards_rect.height() - card_size_.height()) / 2;
            
            if (panel->is_selected()) {
                top -= 10;
            }
            
            panel->move(left + card_space * i, top);
        } else {
            int left = cards_rect.left() + (cards_rect.width() - card_size_.width()) / 2;
            int top = cards_rect.top() + (cards_rect.height() - (cards_list.size() - 1) * card_space - card_size_.height()) / 2;
            
            panel->move(left, top + card_space * i);
        }
    }
    
    QRect play_a_hand_rect = context_map_[player].play_a_hand_rect;
    Cards last_cards = context_map_[player].last_cards;
    card_space = 24;
    
    if (!last_cards.is_empty()) {
        Card::CardList last_cards_list = last_cards.to_card_list();
        for (int i = 0; i < last_cards_list.size(); ++i) {
            CardPanel* panel = card_map_[last_cards_list[i]];
            
            panel->set_front_side(true);
            panel->raise();
            
            if (context_map_[player].alignment == CardAlignment::kHorizontal) {
                int left = play_a_hand_rect.left() + (play_a_hand_rect.width() - (last_cards_list.size() - 1) * card_space
                                                      - panel->width()) / 2;
                int top = play_a_hand_rect.top() + (play_a_hand_rect.height() - panel->height()) / 2;
                
                panel->move(left + i * card_space, top);
            } else {
                int left = play_a_hand_rect.left() + (play_a_hand_rect.width() - panel->width()) / 2;
                int top = play_a_hand_rect.top();
                
                panel->move(left, top + i * card_space);
            }
        }
    }
}

void MainWindow::on_deal_card() {
    static int current_card_pos = 0;
    
    Player* current_player = game_control_->current_player();
    
    if (current_card_pos >= 100) {
        Card card = game_control_->take_one_card();
        
        current_player->store_dealt_card(card);
                
        Cards cards(card);
        
        dealt_card_process(current_player, cards);
        
        game_control_->set_current_player(current_player->next_player());
        
        current_card_pos = 0;
        
        card_move_step(current_player, current_card_pos);
        
        if (game_control_->take_remaining_cards().cards_count() == 3) {
            timer_->stop();
            
            game_status_process(GameControl::GameStatus::kBiddingCard);
            return;
        }
    }
    
    card_move_step(current_player, current_card_pos);
    
    current_card_pos += 15;
}

void MainWindow::on_player_status_changed(Player* player, GameControl::PlayerStatus status) {
    switch (status) {
        case GameControl::PlayerStatus::kPrepareBidLord:
            if (player == game_control_->user_player()) {
                ui->button_group->select_panel(ButtonGroup::Panel::kBidLord, game_control_->max_bidding_points());
            }
            break;
        case GameControl::PlayerStatus::kPreparePlayAHand:
            break;
        case GameControl::PlayerStatus::kWin:
            break;            
    }
}

void MainWindow::on_bid_lord(Player* player, int points, bool is_first_bidding) {
    PlayerContext context = context_map_[player];
    
    if (points == 0) {
        context.info->setPixmap(QPixmap(":/images/buqinag.png"));
    } else {
        if (is_first_bidding) {
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        } else {
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
    }
    
    context.info->show();
    
    show_animatiion(AnimationType::kBidPoints, points);
}

void MainWindow::on_play_a_hand(Player* player, Cards& cards) {    
    auto it = context_map_.find(player);
    it->last_cards = cards;
    
    hide_player_pending_cards(player);
    
    PlayAHand hand(cards);
    PlayAHand::HandType hand_type = hand.hand_type();
    
    if (hand_type == PlayAHand::HandType::kHandPlane || hand_type == PlayAHand::HandType::kHandPlaneTwoSingle
        || hand_type == PlayAHand::HandType::kHandPlaneTwoPair) {
        show_animatiion(AnimationType::kPlane);
    } else if (hand_type == PlayAHand::HandType::kHandSeqPair) {
        show_animatiion(AnimationType::kSeqPair);
    } else if (hand_type == PlayAHand::HandType::kHandSeqSingle) {
        show_animatiion(AnimationType::kSeqSingle);
    } else if (hand_type == PlayAHand::HandType::kHandBomb) {
        show_animatiion(AnimationType::kBomb);
    } else if (hand_type == PlayAHand::HandType::kHandBombJokers) {
        show_animatiion(AnimationType::kJockerBomb);
    }
    
    update_player_cards(player);
}

void MainWindow::show_animatiion(AnimationType type, int points) {
    switch(type) {
        case AnimationType::kSeqSingle:
            break;
        case AnimationType::kSeqPair:
            break;
        case AnimationType::kPlane:
            break;
        case AnimationType::kJockerBomb:
            break;
        case AnimationType::kBomb:
            break;
        case AnimationType::kBidPoints:
            animation_window_->setFixedSize(160, 98);
            animation_window_->move((width() - animation_window_->width()) / 2, (height() - animation_window_->height()) / 2 - 200);
            animation_window_->show_bid_points(points);
            break;
    }
    
    animation_window_->show();
}

void MainWindow::hide_player_pending_cards(Player* player) {
    auto it = context_map_.find(player);
    if (it != context_map_.end()) {
        if (it->last_cards.is_empty()) {
            it->info->setPixmap(QPixmap(":/images/pass.png"));
            it->info->show();
        } else {
            Card::CardList card_list = it->last_cards.to_card_list();
            for (auto last_cards_it = card_list.begin(); last_cards_it != card_list.end(); ++last_cards_it) {
                card_map_[*last_cards_it]->hide();
            }
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), background_image_);
}
