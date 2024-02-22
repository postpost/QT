#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "demo"
#define FIELDS_NUM 5
#define QR_NUM 3
#define GRAPH_COL_NUM 2

enum fieldsToConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    psw = 3,
    port = 4
};


enum queryType{
    airportList = 0,
    departures = 1,
    arrivals = 2,
    graphMonthly = 3,
    graphDaily = 4
};

class DataBaseController : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseController(QObject *parent = nullptr);
    ~DataBaseController();

    bool status = false;
    QString _airportCode;

    //methods
    void AddDatabase(QString driver, QString dbName = "");
    void ConnectToDb();
    void DisconnectFromDb(QString dbName = "");
    bool GetConnectionStatus();
    QSqlError GetLastError();
    void RequestToDb(QString query, int queryType);
    void ReadDataFromDb(QString query, int queryType);
    QString GetAirportCode(int index);

signals:
    void sig_SendConnectionStatus(bool status);
    void sig_SendQueryStatus(QSqlError error, QString query, int queryType);
    void sig_SendQueryStatusToGraph (QSqlError error, QString query, int queryType);
    void sig_SendReadData(QSqlQueryModel* model, int queryType);

    //FOR GRAPH
    void sig_SendPointsData(QMap<QString, QString> &dailyFlightData);

private:
    //fields
    QSqlDatabase *_database;
    QSqlQuery *_query;
    QSqlQueryModel *_modelList;
    QSqlQueryModel *_modelDirection;
    QVector <QString> _connectionData;
    QVector <double> counts_y;
    QVector <double> months_x;

    //container for days statistics
    QMap<QString, QString> _dailyFlightData;
    QMultiMap<int, int> _dailyData;

    QSqlQueryModel *_modelGraph;

};

#endif // DATABASECONTROLLER_H
