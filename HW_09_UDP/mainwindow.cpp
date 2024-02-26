#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();
    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
        outStr << dateTime;
        udpWorker->SendDatagram(dataToSend);
    });

    //Socket to send a message
    _udpSocket = new UDPworker(this);
    _udpSocket->InitSocketForText();
    connect(_udpSocket, &UDPworker::sig_sendTextToGUI, this, &MainWindow::DisplayMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}

void MainWindow::DisplayMessage(QString text, QHostAddress address, uint32_t size)
{
    ui->te_result->append("Принято сообщение \"" + text + "\" " + " от: " + address.toString()+ QString::number(size));
}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}


void MainWindow::on_pb_sendDatagram_clicked()
{
    QString textData = ui->le_inputText->text();
    QByteArray data;

    QDataStream outStream (&data, QIODevice::WriteOnly);
    outStream << textData;
    _udpSocket->SendTextDatagram(data);
}

