#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QObject>
#include "qcustomplot.h"

enum graphType{
    bar = 1,
    line = 2
};

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QCustomPlot *plot, int graphType);
    void AddPointsToGraph(QVector<double> &x, QVector<double> &y);
    void ClearGraph(QCustomPlot *plot);
    void UpdateGraph(QCustomPlot *plot);

signals:

private:
    QCPBars *_barGraph = nullptr; //annual graph
    QCPGraph *_lineGraph = nullptr; //monthly graph
    int _graphType = 0;

};

#endif // GRAPHMANAGER_H
