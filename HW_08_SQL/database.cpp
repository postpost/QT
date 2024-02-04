#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    //создаем объект, где будем хранить данные для подключения
    dataBase = new QSqlDatabase();



    //создаем для отображения
    view = new QTableView();


}

DataBase::~DataBase()
{
    delete dataBase;
    delete modelAllFilms;
    delete modelComedyAndHorror;
    delete view;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    //добавляем базу данных, к которой будем подключаться
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
    qDebug() << status;
    if (status){
        //создаем объект для сохранения таблицы в модели
        modelComedyAndHorror = new QSqlQueryModel();
        modelAllFilms = new QSqlTableModel(this,*dataBase);
        //QSqlDatabase::database(DB_NAME)
    }
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{
    //что мы здесь сохраняем и зачем, если у нас в экземпляре уже лежит БД?
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
    int reqType = 0;
    if (request.contains("Comedy")||request.contains("Horror"))
        reqType = 1;
    else
        reqType = 2;
    if(!dataBase->open()) {
        switch (reqType){
        case 1:err = modelComedyAndHorror->lastError();
            break;
        case 2: err = modelAllFilms->lastError();
            break;
        }
    }

    emit sig_SendStatusRequest(err);
}

void DataBase::ReadAnswerFromDB(int requestType, QString request){

    modelComedyAndHorror->setQuery(request);
    QStringList headers;
    switch (requestType) {
    case requestAllFilms:
        modelAllFilms->setTable(TABLE_NAME);
        headers.clear();
        headers << "Название" << "Год выпуска" << "Жанр";
        for (int i=0; i< headers.size(); ++i){
            modelAllFilms->setHeaderData(i, Qt::Horizontal, headers.at(i));
        };
        view->setModel(modelAllFilms);
        break;
    case requestComedy: case requestHorrors:
        headers.clear();
        headers << "Название" << "Описание";
        for (int i=0; i< headers.size(); ++i){
            modelAllFilms->setHeaderData(i, Qt::Horizontal, headers.at(i));
        }
        view->setModel(modelComedyAndHorror);
        break;
    }
    emit sig_SendDataFromDB(view, requestType);
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
