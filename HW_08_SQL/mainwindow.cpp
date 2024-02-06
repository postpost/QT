#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receiveData){
        dataForConnect = receiveData;
    });

    /*
     *  Сигнал для подключения к БД + возможные ошибки
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered()
{
    //Отобразим диалоговое окно.
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");
       // auto conn = [&]{dataBase->ConnectToDataBase(dataForConnect);};
       // QtConcurrent::run(conn);
       dataBase->ConnectToDataBase(dataForConnect);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }

}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
    QString request="";
    int reqType;
     ///Тут должен быть код ДЗ
    if (ui->cb_category->currentText() == "Все"){
        request = reqAll;
        reqType = requestType::requestAllFilms;
    }
    else if (ui->cb_category->currentText() == "Ужасы")
    {
        request = reqHorror;
        reqType = requestType::requestHorrors;
    }

    else if (ui->cb_category->currentText() == "Комедия")
    {
        request = reqComedy;
        reqType = requestType::requestComedy;
    }
    dataBase->RequestToDB(request, reqType);
    // auto reqToSend = [&]{dataBase->RequestToDB(request, reqType);};
    // QtConcurrent::run(reqToSend);
}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDB(QSqlQueryModel* model, int typeRequest)
{
    ui->tbv_result->setModel(model);
    if (typeRequest == requestType::requestAllFilms){
         ui->tbv_result->hideColumn(0);
        for (int i=4;i<model->columnCount(); ++i)
            ui->tbv_result->hideColumn(i);
   }
    ui->tbv_result->show();

}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}

void MainWindow::ReceiveStatusRequestToDB(QSqlError err){
    if (err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else if (ui->cb_category->currentText() == "Все")
        dataBase->ReadAnswerFromDB(requestType::requestAllFilms, reqAll);
    else if (ui->cb_category->currentText() == "Комедия")
        dataBase->ReadAnswerFromDB(requestType::requestComedy, reqComedy);
    else if (ui->cb_category->currentText() == "Ужасы")
        dataBase->ReadAnswerFromDB(requestType::requestHorrors, reqHorror);
}


