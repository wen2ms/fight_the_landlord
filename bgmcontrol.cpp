#include "bgmcontrol.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

#include "playahand.h"

BGMControl::BGMControl(QObject *parent) : QObject{parent} {
    init_play_list();
    
    for (int i = 0; i < 5; ++i) {
        QMediaPlayer* player = new QMediaPlayer(this);
        QAudioOutput* audio_output = new QAudioOutput(this);
        media_current_index_[i] = 0;
        
        if (i == 2) {
            connect(player, &QMediaPlayer::mediaStatusChanged, this, [=] (QMediaPlayer::MediaStatus status) {
                media_current_index_[i] = (media_current_index_[i] + 1) % play_list_[i].size();
                player->setSource(play_list_[i][media_current_index_[i]]);
                player->play();
            });
        }
        
        audio_output->setVolume(1);
        player->setAudioOutput(audio_output);
        player->setSource(play_list_[i][media_current_index_[i]]);
        
        media_player_.push_back(player);
        audio_output_.push_back(audio_output);
    }
}

void BGMControl::init_play_list() {
    QStringList list;
    list << "Man" << "Woman" << "BGM" << "Other" << "Ending";
    
    QFile infile(":/conf/playList.json");
    infile.open(QFile::ReadOnly);
    
    QByteArray json = infile.readAll();
    infile.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    
    for (int i = 0; i < list.size(); ++i) {
        QString key = list.at(i);
        QJsonArray array = obj.value(key).toArray();
        for (int j = 0; j < array.size(); ++j) {
            play_list_[i].push_back(QUrl(array.at(j).toString()));
        }
    }
}

void BGMControl::start_bgm(int volume) {
    media_current_index_[2] = 0;
    audio_output_[2]->setVolume(volume);
    media_player_[2]->setSource(play_list_[2][media_current_index_[2]]);
    media_player_[2]->play();
}

void BGMControl::stop_bgm() {
    media_player_[2]->stop();
}

void BGMControl::player_bidding_music(int points, PlayerSex sex, bool is_first) {
    int index = (sex == PlayerSex::kMale ? 0 : 1);
    if (is_first && points > 0) {
        media_current_index_[index] = CardType::kOrder;
    } else if (points == 0) {
        if (is_first) {
            media_current_index_[index] = CardType::kNoOrder;
        } else {
            media_current_index_[index] = CardType::kNoRob;
        }
    } else if (points == 2) {
        media_current_index_[index] = CardType::kRob1;
    } else if (points == 3) {
        media_current_index_[index] = CardType::kRob2;
    }
    
    media_player_[index]->setSource(play_list_[index][media_current_index_[index]]);
    media_player_[index]->play();
}

void BGMControl::play_card_music(Cards& cards, PlayerSex sex, bool is_first) {
    int index = (sex == PlayerSex::kMale ? 0 : 1);
    int& current_index = media_current_index_[index];
    
    Card::CardRank rank = Card::CardRank::kRankBegin;
    PlayAHand hand(cards);
    PlayAHand::HandType hand_type = hand.hand_type();
    
    if (hand_type == PlayAHand::HandType::kHandSingle || hand_type == PlayAHand::HandType::kHandPair
        || hand_type == PlayAHand::HandType::kHandTriple) {
        rank = cards.take_random_card().rank();
    }
    
    int type_index = 0;
    switch(hand_type) {
        case PlayAHand::HandType::kHandSingle:
            type_index = rank - 1;
            break;
        case PlayAHand::HandType::kHandPair:
            type_index = rank - 1 + 15;
            break;
        case PlayAHand::HandType::kHandTriple:
            type_index = rank - 1 + 15 + 13;
            break;
        case PlayAHand::HandType::kHandTripleSingle:
            type_index = CardType::kThreeBindOne;
            break;
        case PlayAHand::HandType::kHandTriplePair:
            type_index = CardType::kThreeBindPair;
            break;
        case PlayAHand::HandType::kHandPlane:
        case PlayAHand::HandType::kHandPlaneTwoSingle:
        case PlayAHand::HandType::kHandPlaneTwoPair:
            type_index = CardType::kPlane;
            break;
        case PlayAHand::HandType::kHandSeqPair:
            type_index = CardType::kSequencePair;
            break;
        case PlayAHand::HandType::kHandSeqSingle:
            type_index = CardType::kSequence;
            break;
        case PlayAHand::HandType::kHandBomb:
            type_index = CardType::kBomb;
            break;
        case PlayAHand::HandType::kHandBombJokers:
            type_index = CardType::kJokerBomb;
            break;
        case PlayAHand::HandType::kHandBombSingle:
        case PlayAHand::HandType::kHandBombPair:
        case PlayAHand::HandType::kHandBombTwoSingle:
        case PlayAHand::HandType::kHandBombJokersSingle:
        case PlayAHand::HandType::kHandBombJokersPair:
        case PlayAHand::HandType::kHandBombJokersTwoSingle:
            type_index = CardType::kFourBindTwo;
            break;
        default:
            break;
    }
    
    if (!is_first && (type_index >= CardType::kPlane && type_index < CardType::kFourBindTwo)) {
        current_index = CardType::kMoreBiger1 + QRandomGenerator::global()->bounded(2);
    } else {
        current_index = type_index;
    }
    
    media_player_[index]->setSource(play_list_[index][media_current_index_[index]]);
    media_player_[index]->play();
}
