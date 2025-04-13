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
        media_current_index[i] = 0;
        
        if (i == 2) {
            connect(player, &QMediaPlayer::mediaStatusChanged, this, [=] (QMediaPlayer::MediaStatus status) {
                media_current_index[i] = (media_current_index[i] + 1) % play_list_[i].size();
                player->setSource(play_list_[i][media_current_index[i]]);
                player->play();
            });
        }
        
        audio_output->setVolume(1);
        player->setAudioOutput(audio_output);
        player->setSource(play_list_[i][media_current_index[i]]);
        
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
