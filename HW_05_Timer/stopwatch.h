#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <QTime>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    void StartTimer();
    void StopTimer();
    void ClearTimer();
    int GetTimerId();

private:
    QTimer* timer;
    int id;

signals:
    void sig_StartTimer();
    void sig_StopTimer();
    void sig_ClearTimer();
};

#endif // STOPWATCH_H
