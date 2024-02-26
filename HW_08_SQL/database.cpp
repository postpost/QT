#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    //создаем объект, где будем хранить данные для подключения
    dataBase = new QSqlDatabase();
    modelAllFilms = nullptr;
    modelComedyAndHorror = nullptr;

}

DataBase::~DataBase()
{
    delete dataBase;
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
    dataBase->open( );
    qDebug() << status;

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
void DataBase::RequestToDB(QString request, int requestType)
{
    //сначала формируем запрос к БД
    QSqlError err;
    switch (requestType){
        case 1:
            if (modelAllFilms !=nullptr){
            delete modelAllFilms;
            }
            modelAllFilms = new QSqlTableModel(this, *dataBase);
            modelAllFilms->setQuery(request, *dataBase);
            err = modelAllFilms->lastError();
            break;
        case 2: case 3:
            if (modelComedyAndHorror !=nullptr){
                delete modelComedyAndHorror;
            }
            modelComedyAndHorror = new QSqlQueryModel();
            modelComedyAndHorror->setQuery(request, *dataBase);
            err = modelComedyAndHorror->lastError();
            break;
        }
    emit sig_SendStatusRequest(err);
}

void DataBase::ReadAnswerFromDB(int requestType, QString request){

    QStringList headers;
    headers << "Название" << "Описание";
    switch (requestType) {
    case requestAllFilms:
        modelAllFilms->setTable(TABLE_NAME);
        modelAllFilms->select();

        for (int i=0; i< headers.size(); ++i){
            modelAllFilms->setHeaderData(i+1, Qt::Horizontal, headers.at(i), Qt:: DisplayRole);
        };
        emit sig_SendDataFromDB(modelAllFilms, requestType);
        break;

    case requestComedy: case requestHorrors:
        for (int i=0; i< headers.size(); ++i){
            modelComedyAndHorror->setHeaderData(i, Qt::Horizontal, headers.at(i));
        };
        emit sig_SendDataFromDB(modelComedyAndHorror, requestType);
        break;
    }
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
