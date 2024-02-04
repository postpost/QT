#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QNetworkInterface>


#define BIND_PORT 12345

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram(QNetworkDatagram datagram);
    void ReadTextDatagram (QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

    QString getServerAddress();

private slots:
    void readPendingDatagrams(void); //чтение датаграммы, служебный слот
    void readTextDatagram ( );

private:
    QUdpSocket* serviceUdpSocket; //отвечает за работу через UDp

signals:
    void sig_sendTimeToGUI(QDateTime data); //сигнал, отправляющий время до главного порта
    void sig_sendTextToGUI(QString text);

};

#endif // UDPWORKER_H
