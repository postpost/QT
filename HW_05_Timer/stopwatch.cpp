#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{

    timer = new QTimer(this);
    time = new QTime();
    newTime = new QElapsedTimer();
    connect(timer, &QTimer::timeout, this, &Stopwatch::StartTimer);
    newTime->restart();

}

Stopwatch::~Stopwatch()
{
    delete time;
    delete newTime;
}

void Stopwatch::StartTimer()
{
    int hours, mins, secs, milisecs;
    secs = newTime->elapsed() / 1000;
    mins = (secs /60) % 60;
    hours = secs / 3600;
    secs %= 60;
    milisecs = newTime->elapsed() % 1000;
    QString str = QString("%1:%2:%3:%4").arg(hours, 2, 10, QLatin1Char('0'))
                             .arg(mins, 2, 10, QLatin1Char('0'))
                             .arg(secs, 2, 10, QLatin1Char('0'))
                             .arg(milisecs, 4, 10, QLatin1Char('0'));
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


