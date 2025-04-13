#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class BGMControl : public QObject {
    Q_OBJECT
  public:
    explicit BGMControl(QObject *parent = nullptr);
    
    void init_play_list();

  signals:
    
  private:
    QVector<QMediaPlayer*> media_player_;
    QVector<QAudioOutput*> audio_output_;
    QVector<QVector<QUrl>> play_list_;
    QVector<int> media_current_index;    
};

#endif  // BGMCONTROL_H
