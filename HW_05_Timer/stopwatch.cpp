#include "stopwatch.h"
#include <QtGlobal>

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);

    timerId = GetTimerId();

    connect(timer, &QTimer::timeout, this, &Stopwatch::UpdateTime);


}

Stopwatch::~Stopwatch()
{

}

void Stopwatch::StartTimer()
{
    timer->start(100);
}

void Stopwatch::StopTimer()
{
    timer->stop();

}

void Stopwatch::ClearTimer()
{
    timer->stop();
    countCircle = 0;
    milisecCount = 0;
    milisecs = 0;
    secs = 0;
    mins = 0;
    hours =0;
}

QString Stopwatch::CalculateCircle()
{
    QString lap="";

    if  (countCircle ==0){
        lap = QString("0%1 : 0%2 : 0%3 : 0%4").arg(hours).arg(mins).arg(secs).arg(milisecs);
        lastLap = milisecCount;
        ++countCircle;
        return lap;
    }
    else {
        int secs_now = secs;
        int mins_now = mins;
        int hours_now = hours;

        lastLap = milisecCount - lastLap;
        int milisec_l = lastLap % 10;
        int secs_last = lastLap/10;
        int mins_last = secs_now/60;
        int hours_last = secs_now/3600;
        lap = QString("0%1 : 0%2 : 0%3 : 0%4").arg(hours_last).arg(mins_last).arg(secs_last).arg(milisec_l);
        lastLap = milisecCount;
    }
    return lap;
}

int Stopwatch::GetTimerId()
{
    timerId = timer->timerId();
    return timerId;
}

void Stopwatch::UpdateTime()
{    
    ++milisecs;
    time = QString("0%1 : 0%2 : 0%3 : 0%4").arg(hours).arg(mins).arg(secs).arg(milisecs);
    if (milisecs > 9){
        milisecs = 0;
        ++secs;
    }
    if (secs > 59){
        secs = 0;
        ++mins;

    }
    if (mins > 59){
        mins =0;
        ++hours;
    }
    ++milisecCount;
    emit sig_StartTimer();
}


