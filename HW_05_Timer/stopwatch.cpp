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
        lastLap = milisecCount;
        ++countCircle;
        lap = QString("0%1 : 0%2 : 0%3 : 0%4").arg(hours).arg(mins).arg(secs).arg(milisecs);
        return lap;
    }
    else {

        int secs_now = secs;
        int mins_now = mins;
        int hours_now = hours;

        lastLap = milisecCount - lastLap;
       // qDebug() << lastLap;
        int milisec_l = lastLap % 10;
        int secs_last = lastLap/10;
        int mins_last = secs_now/60;
        int hours_last = secs_now/3600;
        lap = QString("0%1 : 0%2 : 0%3 : 0%4").arg(hours_last).arg(mins_last).arg(secs_last).arg(milisec_l);

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
    time = QString("0%1 : 0%2 : 0%3 : 0%4").arg(hours).arg(mins).arg(secs).arg(milisecs);
    milisecs++;
    if (milisecs > 10){
        milisecs = 0;
        secs++;
    }
    if (secs > 60){
        mins++;
        secs = 0;
    }
    if (mins > 60){
        mins =0;
        hours++;
    }
    milisecCount++;


    emit sig_StartTimer();
}


