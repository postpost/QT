#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    queryToComedyAndHorror = new QSqlQueryModel();
    tbWidget = new QTableWidget();
    view = new QTableView();


}

DataBase::~DataBase()
{
    delete dataBase;
    delete queryToAllFilms;
    delete queryToComedyAndHorror;
    delete view;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    ///Тут должен быть код ДЗ

    bool status;
    status = dataBase->open( );
    if (status)
        queryToAllFilms = new QSqlTableModel(this, QSqlDatabase::database(DB_NAME));

    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{
    //Тут должен быть код ДЗ
    QSqlError err;
    if(dataBase->open() == false)
        qDebug() << queryToAllFilms->database();
        err = queryToAllFilms->lastError();
    err = queryToComedyAndHorror->lastError();

    emit sig_SendStatusRequest(err);
}

void DataBase::ReadAnswerFromDB(int requestType){

   // int rowCount = 0;
   // int colCount = 0;
    QStringList headers;

    switch (requestType) {
    case requestAllFilms:
        headers.clear();
        headers << "Название" << "Год выпуска" << "Жанр";
        for (int i=0; i< headers.size(); ++i){
            queryToAllFilms->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        view->setModel(queryToAllFilms);
        break;
    case requestComedy: case requestHorrors:
        headers.clear();
        headers << "Название" << "Описание";
        for (int i=0; i< headers.size(); ++i){
            queryToAllFilms->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        view->setModel(queryToComedyAndHorror);
        break;
    }
    emit sig_SendDataFromDB(view, requestType);
    view->show();

}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
