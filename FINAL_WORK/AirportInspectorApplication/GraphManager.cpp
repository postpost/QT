#include "GraphManager.h"


GraphManager::GraphManager(QCustomPlot *plot, int graphType)

{
    _graphType = graphType;
    if (_graphType == graphType::bar)
        _barGraph = new QCPBars(plot->xAxis, plot->yAxis);
    else if (_graphType == graphType::line)
        _lineGraph = new QCPGraph(plot->xAxis, plot->yAxis);
    //масштабирование
    plot->setInteraction(QCP::iRangeZoom, true);
    plot->setInteraction(QCP::iRangeDrag, true);

}

void GraphManager::AddPointsToGraph(QVector<double> &x, QVector<double> &y)
{
    if (_graphType == graphType::bar){
          _barGraph->addData(x, y);
    }
    else {
        _lineGraph->addData(x,y);
    }

}

void GraphManager::ClearGraph(QCustomPlot *plot)
{
   // qDebug() << _graphType;
    if (_graphType == graphType::bar){
            _barGraph->data()->clear();
    }
    else if (_graphType == graphType::line){
           _lineGraph->data()->clear();
    }
    plot->replot();
}

void GraphManager::UpdateGraph(QCustomPlot *plot)
{
    plot->rescaleAxes();
    plot->replot();
}


