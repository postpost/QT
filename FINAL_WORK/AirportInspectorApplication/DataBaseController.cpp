#include "DataBaseController.h"

#include <QDate>
#include <_mingw_mac.h>

DataBaseController::DataBaseController(QObject *parent)
    : QObject{parent}
{
    _database = new QSqlDatabase();


    //nullptr
    _modelList = nullptr;
    _modelDirection = nullptr;
    _modelGraph = nullptr;

    //add fields for the connection
    _connectionData.resize(FIELDS_NUM);
    _connectionData[hostName] = "981757-ca08998.tmweb.ru";
    _connectionData[dbName] = "demo";
    _connectionData[login] = "netology_usr_cpp";
    _connectionData[psw] = "CppNeto3";
    _connectionData[port] = "5432";

}

DataBaseController::~DataBaseController()
{
    delete _database;
   // delete _query; //ATTENTION (TEST: будем сохранять модель в mainwindow)
}

void DataBaseController::AddDatabase(QString driver, QString dbName)
{
    *_database = QSqlDatabase::addDatabase(driver, dbName);
}

void DataBaseController::ConnectToDb()
{
    _database->setHostName(_connectionData[hostName]);
    _database->setDatabaseName(_connectionData[dbName]);
    _database->setUserName(_connectionData[login]);
    _database->setPassword(_connectionData[psw]);
    _database->setPort(_connectionData[port].toInt());

    //открывает БД
    status = _database->open();
    qDebug() << status;
    emit sig_SendConnectionStatus(status);
}

void DataBaseController::DisconnectFromDb(QString dbName)
{
    *_database = QSqlDatabase::database(DB_NAME);
    _database->close();
    status = false;
    emit sig_SendConnectionStatus(status);
}

bool DataBaseController::GetConnectionStatus()
{
    return status;
}

void DataBaseController::RequestToDb(QString query, int queryType)
{
    QSqlError err;
    switch(queryType){
    case queryType::airportList:
        if (_modelList != nullptr)
            delete _modelList;
        _modelList = new QSqlQueryModel( );
        _modelList->setQuery(query, *_database);
        err = _modelList->lastError();
        break;
    case queryType::arrivals: case queryType::departures:
        if (_modelDirection !=nullptr)
            delete _modelDirection;
        _modelDirection = new QSqlQueryModel( );
        _modelDirection->setQuery(query, *_database);
        err = _modelList->lastError();
        break;
    case queryType::graphMonthly: case queryType::graphDaily:
        qDebug() << status;
        if (_modelGraph !=nullptr){
            delete _modelGraph;
        }
        _modelGraph = new QSqlQueryModel( );
        _modelGraph ->setQuery(query, *_database);
        err = _modelList->lastError();
        break;
    default:
        break;
    };

    emit sig_SendQueryStatus(err, query, queryType);
}

void DataBaseController::ReadDataFromDb(QString query, int queryType)
{
    QStringList headers;
    switch(queryType){
    case queryType::airportList:
        emit sig_SendReadData(_modelList, queryType);
        break;

    case queryType::arrivals:
        headers << "Номер рейса" << "Время вылета" << "Аэропорт отправления";
        for (int i=0;i< headers.size(); ++i){
            _modelDirection->setHeaderData(i, Qt::Horizontal, headers.at(i));
        };
        emit sig_SendReadData(_modelDirection, queryType);
        break;

    case queryType::departures:
        headers.clear();
        headers << "Номер рейса" << "Время вылета" << "Аэропорт назначения";
        for (int i=0;i< headers.size(); ++i){
            _modelDirection->setHeaderData(i, Qt::Horizontal, headers.at(i));
        };
        emit sig_SendReadData(_modelDirection, queryType);
        break;

    // case queryType::graphMonthly:
    // {
    //     QString month_str = ""; //get records
    //     int month = 0;
    //     int count = 0;
    //     for (int i =0; i <_modelGraph->rowCount();++i){
    //         count = _modelGraph->record(i).value(0).toInt();
    //         month_str = _modelGraph->record(i).value(1).toString();
    //         month = GetMonth(month_str);
    //         //qDebug() << count << " " << month_str;
    //         counts_y.push_back(count);
    //         months_x.push_back(month);
    //     }

    //     emit sig_SendPointsData(months_x, counts_y);
    //     break;
    // }

    case queryType::graphMonthly:case queryType::graphDaily:
    {
        QString count = "";
        QString day ="";
        int month = 0;
        qDebug() << _modelGraph->rowCount();
        for (int i =0; i <_modelGraph->rowCount();++i){
            day = _modelGraph->record(i).value(1).toString();
            count = _modelGraph->record(i).value(0).toString();
            _dailyFlightData[day] = count;
        }
        emit sig_SendPointsData(_dailyFlightData);
        break;
    }
    default:
        break;
    }
}

QString DataBaseController::GetAirportCode(int index)
{
    return _modelList->record(index).value(1).toString();
}

QSqlError DataBaseController::GetLastError()
{
    return _database->lastError();
}



