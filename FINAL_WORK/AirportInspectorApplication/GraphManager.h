#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QObject>
#include "qcustomplot.h"

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QCustomPlot *plot);
    void AddPointsToGraph(QVector<double> x, QVector<double> y);
    void ClearGraph(QCustomPlot *plot);
    void UpdateGraph(QCustomPlot *plot);

signals:

private:
    QCPBars *_barGraph = nullptr; //annual graph
    QCPGraph *_lineGraph = nullptr; //monthly graph

};

#endif // GRAPHMANAGER_H
