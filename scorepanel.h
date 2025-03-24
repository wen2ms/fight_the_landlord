#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget {
    Q_OBJECT

  public:
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();
    
    void set_scores(int left_score, int right_score, int my_score);

  private:
    Ui::ScorePanel *ui;
};

#endif  // SCOREPANEL_H
