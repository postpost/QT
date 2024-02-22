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


    connect(_database, &DataBaseController::sig_SendConnectionStatus, this, &MainWindow::ConnectionStatusReceived);
    connect(_database,&DataBaseController::sig_SendQueryStatus, this, &MainWindow::QueryStatusReceived);
    //после прочтения СЖЕЧЬ
    connect (_database, &DataBaseController::sig_SendReadData, this, &MainWindow::DisplayResults);
    //метод для переподключения к БД (TO DO!!!)
    connect(_timer, &QTimer::timeout, _database, &DataBaseController::ConnectToDb);

    //сигнал о получении данных для графиков
    connect(_database, &DataBaseController::sig_SendPointsData, this, &MainWindow::PointsDataReceived);

    //ПЕРЕНЕСЕНО ИЗ 22 стр.
    _database->ConnectToDb();
    bool status = _database->GetConnectionStatus();

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
         _airportCode =_database->GetAirportCode(ui->cmb_AirportList->currentIndex());
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

void MainWindow::SetUpQuery(QString airportCode, int queryType)
{
    if (queryType == queryType::arrivals){

        _qrArrivals = QString("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                              "as \"Name\" from bookings.flights f "
                              "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                              "WHERE f.arrival_airport  = '%1'").arg(airportCode);
    }
    else if (queryType == queryType::departures){

        _qrDepartures = QString("SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" "
                                "FROM bookings.flights f "
                                "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                                "WHERE f.departure_airport  = '%1'").arg(airportCode);
    }
    else if (queryType == queryType::graphDaily){
        _qrGraphDaily = QString("SELECT count(flight_no), date_trunc('day', scheduled_departure) "
                                "AS \"Day\" FROM bookings.flights f "
                                "WHERE(scheduled_departure::date > date('2016-08-31') "
                                "AND scheduled_departure::date <= date('2017-08-31')) "
                                "AND ( departure_airport = '%1' "
                                "OR arrival_airport = '%1') "
                                "GROUP BY \"Day\" ").arg(airportCode);
    }

    else if (queryType==queryType::graphMonthly){
        _qrGraphMonthly = QString("SELECT count(flight_no), date_trunc('month', scheduled_departure) "
                             "as \"Month\" FROM bookings.flights f "
                             "WHERE (scheduled_departure::date > date('2016-08-31') AND scheduled_departure::date <= date('2017-08-31')) "
                             "AND (f.departure_airport = '%1' OR f.arrival_airport = '%1') "
                             "GROUP BY \"Month\"").arg(airportCode);
        //f.departure_airport, f.arrival_airport,
        //, f.departure_airport, f.arrival_airport
    }
}

void MainWindow::QueryStatusReceived(QSqlError err, QString query, int queryType)
{
    if (err.text() == "" || err.type() != QSqlError::NoError){
           _database->ReadDataFromDb(query, queryType);
    }
    else {
        _msgBox->setIcon(QMessageBox::Critical);
        _msgBox->setText(err.text());
        _msgBox->exec();
    }
}


void MainWindow::DisplayResults(QSqlQueryModel* model, int queryType)
{
    _model = model;
    switch(queryType) {
    case queryType::airportList:
        ui->cmb_AirportList->setModel(_model);
        ui->cmb_AirportList->setModelColumn(0);
        //ui->cmb_AirportList->model()->sort(Qt::AscendingOrder);
        break;
    case queryType::arrivals: case queryType::departures:
        ui->tbv_FlightsList->setModel(_model);
        ui->tbv_FlightsList->show();
    }
}

void MainWindow::PointsDataReceived(QMap<QString, QString> &dailyFlightData)
{
    _flightGraphs = new FlightGraphs(_airportCode, ui->cmb_AirportList->currentText(), dailyFlightData);
    _flightGraphs->exec();
}


void MainWindow::on_btn_Receive_clicked()
{
     _airportCode =_database->GetAirportCode(ui->cmb_AirportList->currentIndex());
    if (ui->rdbArrivals->isChecked()){
        SetUpQuery(_airportCode, queryType::arrivals);
      // qDebug() <<  _airportCode;
       _database->RequestToDb(_qrArrivals,queryType::arrivals);
    }
    else if (ui->rdbDepartures->isChecked()){
         SetUpQuery(_airportCode, queryType::departures);
        _database->RequestToDb(_qrDepartures, queryType::departures);
    }
}


void MainWindow::on_act_Graphs_triggered()
{
    SetUpQuery(_airportCode, queryType::graphDaily);
    _database->RequestToDb(_qrGraphDaily, queryType::graphDaily);
}



void MainWindow::on_btn_Clear_clicked()
{
    _model->clear();
}

