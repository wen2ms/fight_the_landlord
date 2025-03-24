#include "mainwindow.h"

#include <QPainter>
#include <QRandomGenerator>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    ui->button_group->init_buttons();
    
    ui->button_group->select_panel(ButtonGroup::Panel::kSatrt);
    
    int num = QRandomGenerator::global()->bounded(10);
    QString filepath = QString(":/images/background-%1.png").arg(num + 1);
    background_image_.load(filepath);
    
    this->setWindowTitle("Fight The Landord");
    this->setFixedSize(1080, 720);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    painter.drawPixmap(rect(), background_image_);
}
