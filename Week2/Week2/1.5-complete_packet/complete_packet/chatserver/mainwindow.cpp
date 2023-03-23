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

void MainWindow::initServer()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);

    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    QString servIP;
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            servIP = address.toString();
    }
    QString servInfo = "Connect via->"+ servIP+":";
    servInfo+=QString::number(this->listenSocket->serverPort(),10);
    ui->infoEdit->append(servInfo);

    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void MainWindow::acceptConnection()
{
    this->readWriteSocket =this->listenSocket->nextPendingConnection();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    packer.m_tcpSocket = this->readWriteSocket;
    QObject::connect(&this->packer, SIGNAL(recv(char *)), this, SLOT(recvPacket(char *)));

}
void MainWindow::recvMessage()
{
    QByteArray info = this->readWriteSocket->readAll();
    packer.unpackData(info.data(), info.size());
}

void MainWindow::recvPacket(char *info)
{
    ui->infoEdit->append(info);
}

void MainWindow::on_sendButton_clicked()
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(ui->inputEdit->toPlainText().toUtf8());
    qDebug() << array;
    packer.packData(array->data(), array->size());
    //this->readWriteSocket->write(array->data());
    ui->infoEdit->append(ui->inputEdit->toPlainText());
}
