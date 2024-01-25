#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void ReceiveStartSignal();

private slots:
    void on_pb_start_toggled(bool checked);

    void on_pb_clear_clicked();

    void on_pb_cirlce_clicked();

private:
    Ui::MainWindow *ui;
    Stopwatch* stopwatcher;
    int circle_num = 0;

};
#endif // MAINWINDOW_H
