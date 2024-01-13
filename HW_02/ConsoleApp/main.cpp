#include <QCoreApplication>
#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtSql/QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // QMainWindow window;
    // window.show();
    QTcpServer my_tcpServer;
   // my_tcpServer.newConnection();
    QSqlQuery new_query("SELECT country from some_table");
    while(new_query.next()){
        QString country = new_query.value(0).toString();
    }

    return a.exec();
}
