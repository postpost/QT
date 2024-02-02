#include "stopwatch.h"
#include <QtGlobal>

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    time = new QTime();
    newTime = new QElapsedTimer();
    hours =0, mins = 0, secs =0, milisecs = 0;
    time->setHMS(hours, mins, secs, milisecs);
    timerId = GetTimerId();

    connect(timer, &QTimer::timeout, this, &Stopwatch::UpdateTime);
    newTime->restart();

    firstCircle = new QTime();
    lastCircle = new QTime();
}

Stopwatch::~Stopwatch()
{
    delete time;
    delete newTime;
    delete firstCircle;
    delete lastCircle;
}

void Stopwatch::StartTimer()
{
    // if (timerId == GetTimerId())
    //     *time= QTime::fromString(timeStr, "hh:mm:ss.zzz");
    timer->start(100);

}

void Stopwatch::StopTimer()
{
    timer->stop();
   //timeStr = time->toString("hh:mm:ss.zzz");
}

void Stopwatch::ClearTimer()
{
     timer->stop();
     firstCircle->setHMS(0,0,0,0);
     countCircle = 0;
     newTime->restart();
}

QString Stopwatch::StartCircle()
{
    if (countCircle == 0){
        QString firstCircleStr = time->toString("hh:mm:ss.zzz");
        *firstCircle = QTime::fromString(firstCircleStr, "hh:mm:ss.zzz");
    }
    else if (countCircle >=1){
        QString lastCircleStr = time->toString("hh:mm:ss.zzz");
        *lastCircle = QTime::fromString(lastCircleStr, "hh:mm:ss.zzz");
        //lastCircle = time; //здесь два объекта ссылаются на одну и ту же память, это нормально?
        quint32 hourDiff = lastCircle->hour() - firstCircle->hour();
        quint32 minuteDiff = lastCircle->minute() - firstCircle->minute();
        quint32 secDiff = lastCircle->second() - firstCircle->second();
        quint32 msDiff = lastCircle->msec() - firstCircle->msec();
        firstCircle->setHMS(hourDiff, minuteDiff, secDiff, msDiff);
    }
    ++countCircle;
    //qDebug() << firstCircle->toString("hh:mm:ss.zzz");
    return firstCircle->toString("hh:mm:ss.zzz");
    *lastCircle = time->addSecs(-firstCircle->second());

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
    time->setHMS(hours, mins, secs, milisecs);
    emit sig_StartTimer();
}


