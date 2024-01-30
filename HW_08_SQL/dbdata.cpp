#include "dbdata.h"
#include "database.h"
#include "ui_dbdata.h"

DbData::DbData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbData)
{
    ui->setupUi(this);

    //Ресайзим вектор значений, по количеству полей необходимых для
    //подключения к БД
    data.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    data[hostName] = "981757-ca08998.tmweb.ru";
    data[dbName] = "netology_cpp";
    data[port] = "5432";
    data[login] = "netology_usr_cpp";
    data[pass] = "CppNeto3";
    ui->le_host->setText(data[hostName]);
    ui->le_dbName->setText(data[dbName]);
    ui->spB_port->setValue(data[port].toInt());
    ui->le_login->setText(data[login]);
    ui->le_pass->setText(data[pass]);

}

DbData::~DbData()
{
    delete ui;
}
/*!
 * \brief Обработчик кнопки "Ок"
 */
void DbData::on_buttonBox_accepted()
{
    //Добавляем данные в контейнер и передаем в главное окно
    data[hostName] = ui->le_host->text();
    data[dbName] = ui->le_dbName->text();
    data[login] = ui->le_login->text();
    data[pass] = ui->le_pass->text();
    data[port] = ui->spB_port->text();

    emit sig_sendData(data);

}

