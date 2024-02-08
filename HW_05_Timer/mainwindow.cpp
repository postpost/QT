#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_start->setText("Старт");
    ui->pb_clear->setText("Очистить");
    ui->pb_cirlce->setText("Круг");
    ui->lb_time->setText("Время");
    ui->pb_cirlce->setEnabled(false);
    //stopwatch object
    stopwatcher = new Stopwatch(this);
    connect(stopwatcher, &Stopwatch::sig_StartTimer, this, &MainWindow::ReceiveStartSignal);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ReceiveStartSignal()
{
    ui->lb_time->setText(stopwatcher->time);
}

void MainWindow::on_pb_start_toggled(bool checked)
{

    if (checked){
        ui->pb_start->setText("Стоп");
        stopwatcher->StartTimer();
    }
    else{
        ui->pb_start->setText("Старт");
        stopwatcher->StopTimer();
    }
    ui->pb_cirlce->setEnabled(true);
}


void MainWindow::on_pb_clear_clicked()
{
    stopwatcher->ClearTimer();
    ui->txtB_display->clear();
    ui->pb_start->setChecked(false);
    circle_num = 0;
    ui->lb_time->setText("00:00:00:0000");
    ui->pb_cirlce->setEnabled(false);
}


void MainWindow::on_pb_cirlce_clicked()
{
    QString circleDiff = stopwatcher->CalculateCircle();
    ui->txtB_display->append("Круг " + QString::number(++circle_num)
                             + ", время: " + circleDiff
                             + " сек.");
}

