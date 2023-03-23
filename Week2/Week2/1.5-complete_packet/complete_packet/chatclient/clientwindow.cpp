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
    packer.m_tcpSocket = this->readWriteSocket;
    QObject::connect(&this->packer, SIGNAL(recv(char *)), this, SLOT(recvPacket(char *)));

}

void clientWindow::recvMessage()
{
    QByteArray info = this->readWriteSocket->readAll();
    packer.unpackData(info.data(), info.size());
}

void clientWindow::recvPacket(char *info)
{
    ui->infoEdit->append(info);
}



void clientWindow::on_sendButton_clicked()
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(ui->inputEdit->toPlainText().toUtf8());
    qDebug() << array;
    packer.packData(array->data(), array->size());
    //this->readWriteSocket->write(array->data());
    ui->infoEdit->append(ui->inputEdit->toPlainText());
}

