#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    ui->button_group->init_buttons();
    
    ui->button_group->select_panel(ButtonGroup::Panel::kSatrt);
}

MainWindow::~MainWindow() {
    delete ui;
}
