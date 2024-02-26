#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QNetworkInterface>


#define BIND_PORT 12345
#define TEXT_PORT 2424

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void InitSocketForText();
    void ReadDatagram(QNetworkDatagram datagram);
    void SendDatagram(QByteArray data);

    void ReadTextDatagram (QNetworkDatagram datagram);
    void SendTextDatagram(QByteArray& baData);


private slots:
    void readPendingDatagrams(void); //чтение датаграммы, служебный слот
    void processTextDatagram ( );

private:
    QUdpSocket* serviceUdpSocket; //отвечает за работу через UDp
    QUdpSocket* _textSocket;
    uint32_t _textSize = 0;

signals:
    void sig_sendTimeToGUI(QDateTime data); //сигнал, отправляющий время до главного порта
    void sig_sendTextToGUI(QString text, QHostAddress address, uint32_t size);

};

#endif // UDPWORKER_H
