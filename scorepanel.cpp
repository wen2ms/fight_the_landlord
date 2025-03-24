#include "scorepanel.h"

#include "ui_scorepanel.h"

ScorePanel::ScorePanel(QWidget *parent) : QWidget(parent), ui(new Ui::ScorePanel) {
    ui->setupUi(this);
}

ScorePanel::~ScorePanel() {
    delete ui;
}

void ScorePanel::set_scores(int left_score, int right_score, int my_score) {
    ui->left_score->setText(QString::number(left_score));
    ui->right_score->setText(QString::number(right_score));
    ui->my_score->setText(QString::number(my_score));
}
