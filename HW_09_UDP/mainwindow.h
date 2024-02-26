#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "UdpWorker.h"

#define TIMER_DELAY 1000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_start_clicked();
    void DisplayTime(QDateTime data);
    void DisplayMessage(QString text, QHostAddress address, uint32_t size);
    void on_pb_stop_clicked();

    void on_pb_sendDatagram_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    UDPworker* udpWorker;
    UDPworker* _udpSocket;
    uint32_t counterPck = 0;
    uint32_t textSize = 0;

signals:
    void sig_SendTextDatagram(QByteArray data);

};
#endif // MAINWINDOW_H
