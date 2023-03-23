#include "clientwindow.h"
#include "ui_clientwindow.h"

clientWindow::clientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::clientWindow)
{
    ui->setupUi(this);
}

clientWindow::~clientWindow()
{
    delete ui;
}

void clientWindow::connectHost()
{
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress("192.168.0.105"),8888);
//    this->readWriteSocket->connectToHost(QHostAddress::Any,8888);
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void clientWindow::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    ui->infoEdit->append(info);
}


void clientWindow::on_sendButton_clicked()
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(ui->inputEdit->toPlainText().toUtf8());
    this->readWriteSocket->write(array->data());
    ui->infoEdit->append(ui->inputEdit->toPlainText());
    ui->inputEdit->clear();

}
