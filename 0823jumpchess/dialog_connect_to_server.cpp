#include "dialog_connect_to_server.h"
#include "ui_dialog_connect_to_server.h"

Dialog_connect_to_server::Dialog_connect_to_server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_connect_to_server)
{
    ui->setupUi(this);
}

Dialog_connect_to_server::~Dialog_connect_to_server()
{
    delete ui;
}

QString Dialog_connect_to_server::getServerIP()
{
    QString ip=ui->lineEdit->text();
    return ip;
}
