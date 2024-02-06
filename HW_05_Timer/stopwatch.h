#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    ~Stopwatch();

    QString time = "";
    uint32_t milisecCount = 0;
    uint32_t lastLap = 0;
    uint32_t lapPoint = 0;
    //методы
    void StartTimer();
    void StopTimer();
    void ClearTimer();

    QString CalculateCircle();

    int GetTimerId();



private:

    int hours = 0, mins = 0, secs = 0, milisecs = 0;

    int countCircle = 0;
    int timerId;
    QTimer* timer;


public slots:
    void UpdateTime();

signals:
    void sig_StartTimer();

};

#endif // STOPWATCH_H
