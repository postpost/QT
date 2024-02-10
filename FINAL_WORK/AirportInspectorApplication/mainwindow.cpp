#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _database = new DataBaseController(this);
    _msgBox = new QMessageBox(this);
    _timer = new QTimer (this);

    //SETTINGS FOR UI
    ui->lbl_ConnectionStatus->setText("Отключено");
    ui->rdbArrivals->setChecked(true);
    ConfigureDateTime();

    //CONNECT TO DATABASE
    _database->AddDatabase(POSTGRE_DRIVER, DB_NAME);
    _database->ConnectToDb();
    bool status = _database->GetConnectionStatus();

    connect(_database, &DataBaseController::sig_SendConnectionStatus, this, &MainWindow::ConnectionStatusReceived);
    connect(_database,&DataBaseController::sig_SendQueryStatus, this, &MainWindow::QueryStatusReceived);
    //после прочтения СЖЕЧЬ
    connect (_database, &DataBaseController::sig_SendReadData, this, &MainWindow::DisplayResults);
    //метод для переподключения к БД
    connect(_timer, &QTimer::timeout, _database, &DataBaseController::ConnectToDb);

    ConnectionStatusReceived(status);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectionStatusReceived(bool status)
{
    if (status){
        ui->lbl_ConnectionStatus->setText("Подключено");
        ui->lbl_ConnectionStatus->setStyleSheet("color:green");
        LoadInitialDataList(); //загружает первоначальные данные
        //второй лист тут же, мб нужна будет многопоточка
     }

    else {
        ui->lbl_ConnectionStatus->setText("Отключено");
        ui->lbl_ConnectionStatus->setStyleSheet("color:red");

        //TO DO: повторить попытку подключения (timer)
        _database->DisconnectFromDb(DB_NAME);
        //show msg with the error
        _msgBox->setIcon(QMessageBox::Critical);
        _msgBox->setText(_database->GetLastError().text());
        ui->lbl_ConnectionStatus->setText("Отключено");
        ui->lbl_ConnectionStatus->setStyleSheet("color:red");
        _msgBox->exec();

        _timer->start(5000);
    }
}

void MainWindow::LoadInitialDataList()
{
    _database->RequestToDb(_qrAirportList, queryType::airportList);
}


void MainWindow::SendRequestToDb(QString query)
{
    //_database->RequestToDb(query);
}


void MainWindow::ConfigureDateTime()
{
    ui->dateSelect->setMinimumDate(QDate(2016,8,15));
    ui->dateSelect->setMaximumDate(QDate(2017,9,14));
    ui->dateSelect->setDisplayFormat("yyyy.MM.dd");

    QCalendarWidget _calWidget;
    ui->dateSelect->setCalendarWidget(&_calWidget);
    ui->dateSelect->setCalendarPopup(true);

    ui->dateSelect->setDate(QDate(2017,9,14));

}

void MainWindow::SetQuery(QString airportCode, int queryType)
{
    if (queryType == queryType::arrivals){

        _qrArrivals = QString("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                              "as \"Name\" from bookings.flights f "
                              "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                              "WHERE f.arrival_airport  = '%1'").arg(airportCode);
    }
    else if (queryType == queryType::departures){

        _qrDepartures = QString("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                              "as \"Name\" from bookings.flights f "
                              "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                              "WHERE f.arrival_airport  = '%1'").arg(airportCode);
    }
}

void MainWindow::QueryStatusReceived(QSqlError err, QString query, int queryType)
{
    if (!err.NoError){
           _database->ReadDataFromDb(query, queryType);
    }
    else {
        _msgBox->setIcon(QMessageBox::Warning);
        _msgBox->setText(err.text());
        _msgBox->exec();
    }
}


void MainWindow::DisplayResults(QSqlQueryModel* model, int queryType)
{
    switch(queryType) {
    case queryType::airportList:
        ui->cmb_AirportList->setModel(model);
        ui->cmb_AirportList->setModelColumn(0);
        //ui->cmb_AirportList->model()->sort(Qt::AscendingOrder);
        break;
    case queryType::arrivals: case queryType::departures:
        ui->tbv_FlightsList->setModel(model);
        ui->tbv_FlightsList->show();
    }

 //   ui->tbv_FlightsList->setModel(model);
}

void MainWindow::on_btn_Receive_clicked()
{
     _airportCode =_database->GetAirportCode(ui->cmb_AirportList->currentIndex());
    if (ui->rdbArrivals->isChecked()){
        SetQuery(_airportCode, queryType::arrivals);
      // qDebug() <<  _airportCode;
       _database->RequestToDb(_qrArrivals,queryType::arrivals);
    }
    else if (ui->rdbDepartures->isChecked()){
         SetQuery(_airportCode, queryType::departures);
        _database->RequestToDb(_qrDepartures,queryType::departures);
    }
}

