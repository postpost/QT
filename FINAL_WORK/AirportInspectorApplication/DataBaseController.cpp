#include "DataBaseController.h"

DataBaseController::DataBaseController(QObject *parent)
    : QObject{parent}
{
    _database = new QSqlDatabase();
    _query = new QSqlQuery();

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

    //открывыает БД
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
        if (_modelList!=nullptr)
            delete _modelList;
        _modelList = new QSqlQueryModel( );
        _modelList->setQuery(query, *_database);
         err = _modelList->lastError();
        break;
    case queryType::arrivals: case queryType::departures:
        if (_modelDirection != nullptr)
            delete _modelDirection;
        _modelDirection = new QSqlQueryModel( );
        _modelDirection->setQuery(query, *_database);
        err = _modelDirection->lastError();
        }
    emit sig_SendQueryStatus(err, query, queryType);
}

void DataBaseController::ReadDataFromDb(QString query, int queryType)
{
    switch(queryType){
    case queryType::airportList:
        emit sig_SendReadData(_modelList, queryType);
        break;
    case queryType::arrivals: case queryType::departures:
        QStringList headers;
        headers << "Номер рейса" << "Время вылета" << "Аэропорт отправления";
        for (int i=0;i< headers.size(); ++i){
            _modelDirection->setHeaderData(i, Qt::Horizontal, headers.at(i));

        }
        emit sig_SendReadData(_modelDirection, queryType);
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



