#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QObject>
#include "qcustomplot.h"

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent = nullptr);

signals:
};

#endif // GRAPHMANAGER_H
