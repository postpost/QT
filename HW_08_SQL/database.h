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
    void RequestToDB(QString request);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void ReadAnswerFromDB(int answerType);


signals:

   void sig_SendDataFromDB(QTableView* view, int typeR);
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError error);



private:

    QSqlDatabase* dataBase; //для подключения к БД
    QSqlQueryModel* queryToComedyAndHorror; //запрос на получение комедий и ужасов
    QSqlTableModel* queryToAllFilms; //запрос на получение ВСЕХ фильмов
    QTableWidget* tbWidget; //для отображения таблицы
    QTableView *view; //для отображения таблицы

};

#endif // DATABASE_H
