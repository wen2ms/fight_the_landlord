#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget {
    Q_OBJECT

  public:
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();
    
    void init_buttons();

  private:
    Ui::ButtonGroup *ui;
};

#endif  // BUTTONGROUP_H
