#include "dialog_create_connection.h"
#include "ui_dialog_create_connection.h"

Dialog_create_connection::Dialog_create_connection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_create_connection)
{
    ui->setupUi(this);
}

Dialog_create_connection::~Dialog_create_connection()
{
    delete ui;
}

void Dialog_create_connection::set_the_label(QString in)
{
    QString output="Host IP:   "+in;
    ui->label->setText(output);
}

