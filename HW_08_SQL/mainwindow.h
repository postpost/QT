#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include "database.h"
#include "dbdata.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void ScreenDataFromDB(QSqlQueryModel* model, int typeRequest);
    void ReceiveStatusConnectionToDB(bool status);
    void ReceiveStatusRequestToDB(QSqlError err);


private slots:
    void on_act_addData_triggered();
    void on_act_connect_triggered();
    void on_pb_request_clicked();


    void on_pb_clear_clicked();

signals:
    void sig_RequestToDb(QString request);

private:

    QVector<QString> dataForConnect; //Данные для подключения к БД.

    Ui::MainWindow *ui;
    DbData *dataDb; //диалоговое окно для подключения к БД
    DataBase* dataBase; //класс для работы с БД
    QMessageBox* msg;
    QSqlQueryModel *_modelAllFilms; //запрос на получение ВСЕХ фильмов
    QSqlQueryModel* _modelComedyAndHorror; //запрос на получение комедий и ужасов



    QString reqAll = "SELECT title, release_year, c.name  FROM film f "
                      "JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id  = fc.category_id";

    QString reqComedy = "SELECT title, description "
                      "FROM film f JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id = fc.category_id "
                      "WHERE c.name = 'Comedy'";

    QString reqHorror = "SELECT title, description "
                        "FROM film f JOIN film_category fc on f.film_id = fc.film_id "
                        "JOIN category c on c.category_id = fc.category_id "
                        "WHERE c.name = 'Horror'";




};
#endif // MAINWINDOW_H
