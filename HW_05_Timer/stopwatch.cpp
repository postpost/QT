#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Stopwatch::StartTimer);

}

void Stopwatch::StartTimer()
{
    //timer->setInterval(1000);
    timer->start();
    emit sig_StartTimer();
}

void Stopwatch::StopTimer()
{
    timer->stop();
    emit sig_StopTimer();
}

void Stopwatch::ClearTimer()
{
     timer->stop();
    //timer->disconnect(timer, &QTimer::destroyed, this, &Stopwatch::sig_ClearTimer);
}

int Stopwatch::GetTimerId()
{
    id = timer->timerId();
    return id;
}


