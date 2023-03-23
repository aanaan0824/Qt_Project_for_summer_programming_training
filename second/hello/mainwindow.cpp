#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout,[=](){
        static int i=0;
        i++;
        ui->lcdNumber->display(i);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    Dialog1 *dia=new Dialog1;
    dia->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    if(timer->isActive()==false)timer->start(1000);
}


void MainWindow::on_pushButton_3_clicked()
{
    if(timer->isActive()==true)timer->stop();
}

