#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QSqlRecord>

#include "DataBaseController.h"
#include "GraphManager.h"
#include "FlightGraphs.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ConnectionStatusReceived(bool status);
    void QueryStatusReceived(QSqlError err, QString query, int queryType);
    void DisplayResults(QSqlQueryModel* model, int queryType);
   //void ReceiveDbPointer(DataBaseController * database);

private slots:
    void on_btn_Receive_clicked();
    void on_act_Graphs_triggered();


private:
    Ui::MainWindow *ui;
    DataBaseController *_database;
    QMessageBox *_msgBox;
    QTimer *_timer;
    QString _airportCode;
    QSqlQueryModel *_model;
   // QDate

    //SHOW GRAPHS IN NEW WINDOW
    FlightGraphs *_flightGraphs;


    //methods
    void LoadInitialDataList();
    void SendRequestToDb(QString query);
   // void DisplayDestinations(QString row);
    void AddInitialQueryList();

    void ConfigureDateTime();
    void SetQuery(QString airportCode, int queryType);



    //SQL Queries

    QString _qrAirportList = "SELECT airport_name->>'ru' as \"airportName\","
                                "airport_code FROM bookings.airports_data "
                                "ORDER BY airport_name";

    QString _qrArrivals = "";
        /*QString("SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                            "as \"Name\" from bookings.flights f "
                            "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                  "WHERE f.arrival_airport  = '%1'");*/

    QString _qrDepartures = "";

    /*"SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" "
                          "FROM bookings.flights f "
                          "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                          "WHERE f.departure_airport  = '%1'";*/

};
#endif // MAINWINDOW_H
