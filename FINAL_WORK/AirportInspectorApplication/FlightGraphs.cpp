#include "FlightGraphs.h"
#include "ui_FlightGraphs.h"

FlightGraphs::FlightGraphs(QString airportCode, QString airportName, QMap<QString, QString> &dailyFlightData, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FlightGraphs)
{
    ui->setupUi(this);
    ui->ln_Info->setText(airportName);
    ui->ln_Info->setEnabled(false);
    _msg = new QMessageBox(this);

    SetUpMonthList();

    _grManagerMonthly = new GraphManager(ui->cPlot_Monthly, graphType::bar);
    _grManagerDaily = new GraphManager(ui->cPlot_Daily, graphType::line);


    //Save data into the class
    int month = 0;
    for (auto it = dailyFlightData.keyValueBegin(); it!=dailyFlightData.keyValueEnd();++it){
        _dailyFlightData.insert(it->first, it->second);
        month = GetMonth(it->first);
        _dailyData.insert(month, it->second.toInt());
    }
    connect (this, &FlightGraphs::sig_SendMonthlyData, this, &FlightGraphs::ShowGraph);

    SetUpMonthlyData();
}

FlightGraphs::~FlightGraphs()
{
    delete ui;
    delete _grManagerMonthly;
    delete _grManagerDaily;
}


void FlightGraphs::ShowGraph()
{
    DisplayMonthlyData();
    DisplayDailyData(ui->cmb_MonthList->currentIndex());

}

void FlightGraphs::SetUpMonthList()
{
    _monthList.resize(12);
     _monthList[0] = "Январь";
     _monthList[1] = "Февраль";
     _monthList[2] = "Март";
     _monthList[3] = "Апрель";
     _monthList[4] = "Май";
     _monthList[5] = "Июнь";
     _monthList[6] = "Июль";
     _monthList[7] = "Август";
     _monthList[8] = "Сентябрь";
     _monthList[9] = "Октябрь";
     _monthList[10] = "Ноябрь";
     _monthList[11] = "Декабрь";


     for (auto month:_monthList){
         ui->cmb_MonthList->addItem(month);
     }
}


void FlightGraphs::SetUpMonthlyData()
{
    int month = 0, count = 0;
    int i = 1;
    for (auto [key, value] :_dailyData.asKeyValueRange()) {
        if (key == i){
            count += value;
            _monthlyData[key] = count;
        }
        else {
            count = value;
            i++;
        }
    }

    emit sig_SendMonthlyData();
}

int FlightGraphs::GetMonth(QString dateString)
{
    QStringList dateList = dateString.split( '-' );
    return dateList.at(1).toInt();
}

int FlightGraphs::GetDay(QString dateString)
{
    QStringList dateList = dateString.split( '-');
    dateList = dateList.at(2).split('T');
    return dateList.at(0).toInt();
}

void FlightGraphs::DisplayMonthlyData()
{
    _grManagerMonthly->ClearGraph(ui->cPlot_Monthly);

    ui->cPlot_Monthly->xAxis->setRange(0, 12);
    ui->cPlot_Monthly->xAxis->setLabel("Months");
    ui->cPlot_Monthly->yAxis->setLabel("Flights count");
    ui->cPlot_Monthly->xAxis->setTicks(true);

    //Добавляет лейблы шага сетки по оси Х
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for (int i=0; i < _monthList.size(); ++i){
        textTicker->addTick(i+1, _monthList[i]);
    }
    ui->cPlot_Monthly->xAxis->setTicker(textTicker);

    //ADD STEP
    QVector<double> _xM;
    QVector<double> _yM;
    _xM.resize(_monthlyData.size()+1, 0);
    _yM.resize(_monthlyData.size()+1, 0);

    _xM[0] = 0;
    _yM[0] = 0;

    for (int i = 1; i< _xM.size(); ++i){
        _xM[i] = i;
        _yM[i] = _monthlyData.value(i);
    }
    _grManagerMonthly->AddPointsToGraph(_xM,_yM);
    _grManagerMonthly->UpdateGraph(ui->cPlot_Monthly);
}

void FlightGraphs::DisplayDailyData(int index)
{
    //SHOW DAILY
    _grManagerDaily->ClearGraph(ui->cPlot_Daily);
    //ui->cPlot_Daily->xAxis->setRange(0, 12);
    ui->cPlot_Daily->xAxis->setLabel("Days");
    ui->cPlot_Daily->yAxis->setLabel("Flights count");
    ui->cPlot_Daily->xAxis->setTicks(true);

    QVector<double> _xD;
    QVector<double> _yD;
    int count =0;
    int day = 0, i =0;;

    int month = index+1;
    for (auto [key, value]:_dailyData.asKeyValueRange()){
        if (month == key)
            count++;
        else if (count>0) {
            break;
        }
    }

    _xD.resize(count);
    _yD.resize(count);

    for (auto [key, value]:_dailyFlightData.asKeyValueRange()){
        //qDebug() << key << ": " << value << "\n";
        if (month == GetMonth(key)){
            day = GetDay(key);
            _xD[i] = day;
            _yD[i] = value.toInt();
            i++;
        }
        else if (i>0){
            break;
        }
    }
    _grManagerDaily->AddPointsToGraph(_xD,_yD);
    _grManagerDaily->UpdateGraph(ui->cPlot_Daily);

}


void FlightGraphs::on_btn_Close_clicked()
{
    this->accept();
}


void FlightGraphs::on_cmb_MonthList_currentIndexChanged(int index)
{
    if (index == 0)
        return;
    DisplayDailyData(index);
}

