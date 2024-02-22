#ifndef FLIGHTGRAPHS_H
#define FLIGHTGRAPHS_H

#include <QWidget>
#include "GraphManager.h"

#define GRAPH_NUM 2

enum months{
    jan = 1,
    feb = 2,
    mar = 3,
    apr = 4,
    may = 5,
    june = 6,
    july = 7,
    aug = 8,
    sep = 9,
    oct = 10,
    nov = 11,
    dec = 12
};

namespace Ui {
class FlightGraphs;
}

class FlightGraphs : public QDialog
{
    Q_OBJECT

public:
    explicit FlightGraphs(QString airportCode, QString airportName, QMap<QString, QString> &dailyFlightData, QWidget *parent = nullptr);
    ~FlightGraphs();


public slots:
    void ShowGraph();

signals:
    void sig_SendMonthlyData();

private slots:
    void on_btn_Close_clicked();

    void on_cmb_MonthList_currentIndexChanged(int index);

private:
    Ui::FlightGraphs *ui;
    QMessageBox *_msg;
  //  QList <GraphManager*> _graphManagerList;
    GraphManager *_grManagerMonthly;
    GraphManager *_grManagerDaily;

    //TO GET DATA FROM DB
    QMultiMap<int, int> _dailyData;
    QMap<QString, QString> _dailyFlightData;
    QMap<int, int> _monthlyData;
    QVector <QString> _monthList;


    //methods
    void SetUpDailyData();
    void SetUpMonthlyData();
    void SetUpMonthList();
    int GetMonth(QString dateString);
    int GetDay(QString dateString);

    void DisplayMonthlyData();
    void DisplayDailyData(int index);


};

#endif // FLIGHTGRAPHS_H
