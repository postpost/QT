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
    int GetTimerId();

private:
    QTimer* timer;
    QTime* time;
    QElapsedTimer* newTime;
    int id;

signals:
    void sig_StartTimer();
    void sig_StopTimer();
    void sig_ClearTimer();
};

#endif // STOPWATCH_H
