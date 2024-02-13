#include "GraphManager.h"


GraphManager::GraphManager(QCustomPlot *plot)

{
    _barGraph = new QCPBars(plot->xAxis, plot->yAxis);
    _lineGraph = new QCPGraph(plot->xAxis, plot->yAxis);
    //масштабирование
    plot->setInteraction(QCP::iRangeZoom, true);
    plot->setInteraction(QCP::iRangeDrag, true);

}

void GraphManager::AddPointsToGraph(QVector<double> x, QVector<double> y)
{
    //_barGraph[0].addData(x, y);
    _barGraph->addData(x, y);
    //_lineGraph->addData(x,y);
}

void GraphManager::ClearGraph(QCustomPlot *plot)
{
    _barGraph->data()->clear();
    _lineGraph->data()->clear();
    plot->graph()->data()->clear();
    plot->replot();
}

void GraphManager::UpdateGraph(QCustomPlot *plot)
{
    plot->rescaleAxes();
    plot->replot();
}


