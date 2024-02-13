#include "FlightGraphs.h"
#include "ui_FlightGraphs.h"

FlightGraphs::FlightGraphs(QString airportCode, QString airportName, DataBaseController* database, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FlightGraphs)
{
    ui->setupUi(this);
    SetupQuery(airportCode);
    ui->ln_Info->setText(airportName);
    ui->ln_Info->setEnabled(false);
    _msg = new QMessageBox(this);

    //have to create graphManager here

    // _graphManagerList.resize(GRAPH_NUM);
    // _graphManagerList[0] = new GraphManager(ui->cPlot_Monthly);
    // _graphManagerList[1] = new GraphManager(ui->cPlot_Daily);

    _grManagerMonthly = new GraphManager(ui->cPlot_Monthly);

    _database = database;

    //ПОСЛАТЬ ЗАПРОС В БД
    //ПОЛУЧИТЬ ТОЧКИ
    //ЗАПОЛНИТЬ ТОЧКАМИ СЕРИЮ
    //ДОБАВИТЬ СЕРИЮ НА ГРАФИК (ОТОБРАЗИТЬДАННЫ)

    connect(_database, &DataBaseController::sig_SendQueryStatus, this,&FlightGraphs::QueryStatusReceived);
    connect (_database,&DataBaseController::sig_SendPointsData, this, &FlightGraphs::ShowGraph);
}

FlightGraphs::~FlightGraphs()
{
    delete ui;
}

void FlightGraphs::QueryStatusReceived(QSqlError err, QString query, int queryType)
{
    if (err.text() == "" || err.type() != QSqlError::NoError){
        _database->ReadDataFromDb(_qrMonthly, queryType::graphMonthly);
    }
    else {
        _msg->setIcon(QMessageBox::Critical);
        _msg->setText(err.text());
        _msg->exec();
    }
}

void FlightGraphs::ShowGraph(QVector<double> x, QVector<double> y)
{

    _grManagerMonthly->ClearGraph(ui->cPlot_Monthly);

    ui->cPlot_Monthly->xAxis->setRange(1, 12);
    ui->cPlot_Monthly->xAxis->setLabel("Months");
    ui->cPlot_Monthly->yAxis->setLabel("Flights count");

    //ADD STEP
    QVector<double> _x;
    QVector<double> _y;
    _x.resize(x.size()+1, 0);
    _y.resize(y.size()+1, 0);

    _x[0] = 0;
    _y[0] = 0;

    for (int i =1; i< _x.size(); ++i){
        _x[i] = x[i-1];
        _y[i] = y[i-1];
    }

    // ui->cPlot_Monthly->addGraph();
    // ui->cPlot_Monthly->graph(0)->setData(x, y);

    _grManagerMonthly->AddPointsToGraph(_x,_y);
    _grManagerMonthly->UpdateGraph(ui->cPlot_Monthly);
}

void FlightGraphs::SetupQuery(QString airportCode)
{
    _qrMonthly = QString("SELECT count(flight_no), date_trunc('month', scheduled_departure) "
                "as \"Month\" FROM bookings.flights f "
                "WHERE (scheduled_departure::date > date('2016-08-31') AND scheduled_departure::date <= date('2017-08-31')) "
                "AND (f.departure_airport = '%1' OR f.arrival_airport = '%1') "
                "GROUP BY \"Month\"").arg(airportCode);
    //f.departure_airport, f.arrival_airport,
    //, f.departure_airport, f.arrival_airport
}

void FlightGraphs::on_btn_Close_clicked()
{
    // emit sig_SendDbPointer(_database);
    // _database = nullptr;
    this->accept();
}


void FlightGraphs::on_btn_Display_clicked()
{
    _database->RequestToDb(_qrMonthly, queryType::graphMonthly);
}

