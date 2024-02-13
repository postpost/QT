#ifndef FLIGHTGRAPHS_H
#define FLIGHTGRAPHS_H

#include <QWidget>
#include "GraphManager.h"
#include "DataBaseController.h"

#define GRAPH_NUM 2

namespace Ui {
class FlightGraphs;
}

class FlightGraphs : public QDialog
{
    Q_OBJECT

public:
    explicit FlightGraphs(QString airportCode, QString airportName, DataBaseController* database, QWidget *parent = nullptr);
    ~FlightGraphs();


public slots:
    void QueryStatusReceived (QSqlError err, QString query, int queryType);
    void ShowGraph(QVector<double> x, QVector<double> y);

signals:
    void sig_SendDbPointer(DataBaseController* _database);

private slots:
    void on_btn_Close_clicked();

    void on_btn_Display_clicked();

private:
    Ui::FlightGraphs *ui;
    QMessageBox *_msg;
  //  QList <GraphManager*> _graphManagerList;
    GraphManager *_grManagerMonthly;

    //TO GET DATA FROM DB
    DataBaseController *_database;

    QString _qrMonthly ="";
    QString _qrDaily = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" "
                           "FROM bookings.flights f"
                           "WHERE(scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                           "AND ( departure_airport = airportCode or arrival_airport = airportCode) "
                           "GROUP BY \"Day\"";

    //methods
    void SetupQuery(QString airportCode);


};

#endif // FLIGHTGRAPHS_H
