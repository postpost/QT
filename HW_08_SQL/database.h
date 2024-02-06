#ifndef DATABASE_H
#define DATABASE_H

//Класс, отвечающий за работу с БД: подключение, отключение, запросы, редактирование, отображение

#include <QTableWidget>
#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QtConcurrent>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"
#define TABLE_NAME "film"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

//Типы запросов
enum requestType{

    requestAllFilms = 1,
    requestComedy   = 2,
    requestHorrors  = 3

};

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(QString request, int requestType);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void ReadAnswerFromDB(int requestType, QString request);




signals:

   void sig_SendDataFromDB(QSqlQueryModel* model, int typeR);
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError error);



private:

    QSqlDatabase* dataBase; //для подключения к БД
    QSqlTableModel* modelAllFilms; //запрос на получение ВСЕХ фильмов
    QSqlQueryModel* modelComedyAndHorror; //запрос на получение комедий и ужасов

};

#endif // DATABASE_H
