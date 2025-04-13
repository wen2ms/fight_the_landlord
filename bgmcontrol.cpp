#include "bgmcontrol.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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
