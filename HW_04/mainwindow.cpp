#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->rb_list1->setChecked(true);
    ui->pgB_level->setValue(0);
    ui->pgB_level->setRange(0, 100);
   // ui->pgB_level->setMaximum(100);
    ui->pb_increase->setText("Увеличить");
    ui->pb_increase->setChecked(false);
    ui->pb_increase->setStyleSheet("background : #CD96CD");
    //customize progress bar
    QColor barColor = QColor(255, 55, 0);
    QString style = QString("QProgressBar::"
                            "chunk {background-color: #CD96CD; "
                            "width: 20px; "
                            "margin: 0.5px;}").arg(barColor.name());
    ui->pgB_level->setStyleSheet(style);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rb_list1_clicked()
{
    if (ui->cmb_list->count()>0)
        ui->cmb_list->clear();
     if (ui->rb_list1->isChecked() && ui->cmb_list->count()==0){
        for (int i=1; i<= 10; ++i){
            ui->cmb_list->addItem("Список " + QString::number(i));
        }
    }
}


void MainWindow::on_rb_list2_clicked()
{
    if (ui->rb_list2->isChecked()){
        ui->cmb_list->clear();
        for (int i=1; i<= 10; ++i){
            ui->cmb_list->addItem("Элемент " + QString::number(i));
        }
    }
}


void MainWindow::on_pb_increase_toggled(bool checked)
{
    int step =10;
    ui->pgB_level->setValue(val+=step);
    if (val > 100){
        //ui->pgB_level->setValue(0);
        ui->pgB_level->reset();
        val =0;
    }

}

