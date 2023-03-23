#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug("Total number of clicks: %d\n", this->ui->pushButton->getNumClicks());
    QString str;

    str = str.setNum(this->ui->pushButton->getNumClicks());
    str.insert(0, "Total clicked times: ");
    ui->label->setText(str);

}
