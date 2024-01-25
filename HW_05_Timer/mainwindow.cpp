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
    time.setHMS(0, 0, 0, 0);

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
    stopwatcher->StartTimer();
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

}


void MainWindow::on_pb_clear_clicked()
{
    newTime.restart();
    stopwatcher->ClearTimer();
    ui->txtB_display->clear();
    ui->pb_start->setChecked(false);
    circle_num = 0;
   // ui->lb_time->killTimer(stopwatcher->GetTimerId());
    ui->lb_time->setText("00:00:00:0000");

}


void MainWindow::on_pb_cirlce_clicked()
{
    QTime last_time= QTime::fromString(ui->lb_time->text());
    QString str = ui->lb_time->text();
    circleTime = QTime::fromString(str);
    circleTime = last_time.addSecs(-time.second());
    //QTime::fromString(str);
    //qDebug() << str << " " << circleTime.second();
    circleTime.setHMS(time.hour(), time.minute(), time.second());
    qDebug() << circleTime;
    ui->txtB_display->append("Круг " + QString::number(++circle_num)
                                     + ", время: " + ui->lb_time->text()
                                     + " сек.");

}

