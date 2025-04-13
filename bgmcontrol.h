#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>

#include "cards.h"

class BGMControl : public QObject {
    Q_OBJECT
  public:
    enum PlayerSex {kMale, kFemale};
    
    enum CardType
    {
        kThree,
        kFoue,
        kFive,
        kSix,
        kSeven,
        kEight,
        kNine,
        kTen,
        kJack,
        kQueen,
        kKing,
        kAce,
        kTow,
        kSmallJoker,
        kBigJoker,

        kThreeDouble,
        kFoueDouble,
        FiveDouble,
        kSixDouble,
        kSevenDouble,
        kEightDouble,
        kNineDouble,
        kTenDouble,
        kJackDouble,
        kQueenDouble,
        kKingDouble,
        kAceDouble ,
        kTowDouble,
        
        kThreeTriple,
        kFoueTriple,
        kFiveTriple,
        kSixTriple,
        kSevenTriple,
        kEightTriple,
        kNineTriple,
        kTenTriple,
        kJackTriple,
        kQueenTriple,
        kKingTriple,
        kAceTriple,
        kTowTriple,
        
        kPlane,
        kSequencePair,
        kThreeBindOne,
        kThreeBindPair,
        kSequence,
        kFourBindTwo,
        kFourBind2Pair,
        kBomb,
        kJokerBomb,
        kPass1,
        kPass2,
        kPass3,
        kPass4,
        kMoreBiger1,
        kMoreBiger2,
        kBiggest,

        kNoOrder,
        kNoRob, 
        kOrder,
        kRob1, 
        kRob2,
        kLast1, 
        kLast2
    };
        
    explicit BGMControl(QObject *parent = nullptr);
    
    void init_play_list();
    
    void start_bgm(int volume);
    
    void stop_bgm();
    
    void player_bidding_music(int points, PlayerSex sex, bool is_first);
    
    void play_card_music(Cards& cards, PlayerSex sex, bool is_first);

  signals:
    
  private:
    QVector<QMediaPlayer*> media_player_;
    QVector<QAudioOutput*> audio_output_;
    QVector<QVector<QUrl>> play_list_;
    QVector<int> media_current_index_;    
};

#endif  // BGMCONTROL_H
