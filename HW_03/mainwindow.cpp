#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->statusbar->showMessage("Вы справились, поздравляю!");
    dialog = new Dialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btn_ok_cancel_accepted()
{
    dialog->show();
}

