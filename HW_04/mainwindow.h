#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_rb_list1_clicked();

    void on_rb_list2_clicked();

    void on_pb_increase_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    int val =0;
    QPalette palette;
};
#endif // MAINWINDOW_H
