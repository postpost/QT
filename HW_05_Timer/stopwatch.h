#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    ~Stopwatch();
    void StartTimer();
    void StopTimer();
    void ClearTimer();
    void StartCircle();
    int GetTimerId();
    int hours, mins, secs, milisecs;

    QTime* time;
    QTimer* timer;

private:
    QElapsedTimer* newTime;
    int id;

public slots:
    void UpdateTime();

signals:
    void sig_StartTimer();
    void sig_StopTimer();
    void sig_ClearTimer();
};

#endif // STOPWATCH_H
