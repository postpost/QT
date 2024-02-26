#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{
    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    //сигнал испускается, когда сервер присылает данные
    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

void UDPworker::InitSocketForText()
{
    _textSocket = new QUdpSocket(this);
    _textSocket->bind(QHostAddress::LocalHost, TEXT_PORT);
    connect (_textSocket, &QUdpSocket::readyRead, this, &UDPworker::processTextDatagram);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    data = datagram.data(); //считываем данные датаграммы

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}

void UDPworker::ReadTextDatagram(QNetworkDatagram datagram)
{
    auto address = datagram.senderAddress();
    QByteArray data;
    data = datagram.data(); //считываем данные датаграммы
    _textSize = data.size();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QString text;
    inStr >> text;

    emit sig_sendTextToGUI(text, address,_textSize);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendTextDatagram(QByteArray& baData)
{
    _textSize = _textSocket->writeDatagram(baData, QHostAddress::LocalHost, TEXT_PORT);
}

void UDPworker::SendDatagram(QByteArray data)
{
    // Отправляем данные на localhost и задефайненный порт
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    //Производим чтение принятых датаграмм
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }
}

void UDPworker::processTextDatagram()
{
    while (_textSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = _textSocket->receiveDatagram();
        ReadTextDatagram(datagram);
    }
}
