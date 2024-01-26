#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    time = new QTime();
    newTime = new QElapsedTimer();
    hours =0, mins = 0, secs =0, milisecs = 0;
    time->setHMS(hours, mins, secs, milisecs);

    connect(timer, &QTimer::timeout, this, &Stopwatch::UpdateTime);
    newTime->restart();
}

Stopwatch::~Stopwatch()
{
    delete time;
    delete newTime;
}

void Stopwatch::StartTimer()
{
    timer->start(10);
    newTime->start();
}

void Stopwatch::StopTimer()
{
    timer->stop();
    emit sig_StopTimer();
}

void Stopwatch::ClearTimer()
{
     timer->stop();
}

void Stopwatch::StartCircle()
{
    QTime last_time= time->currentTime();


}

int Stopwatch::GetTimerId()
{
    id = timer->timerId();
    return id;
}

void Stopwatch::UpdateTime()
{
    secs = newTime->elapsed() / 1000;
    mins = (secs /60) % 60;
    hours = secs / 3600;
    secs %= 60;
    milisecs = newTime->elapsed() % 1000;
    //++milisecs;
    time->setHMS(hours, mins, secs, milisecs);
    emit sig_StartTimer();

}


