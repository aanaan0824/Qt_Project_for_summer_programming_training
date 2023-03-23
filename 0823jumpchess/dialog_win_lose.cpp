#include "dialog_win_lose.h"
#include "ui_dialog_win_lose.h"

Dialog_win_lose::Dialog_win_lose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_win_lose)
{
    ui->setupUi(this);
}

Dialog_win_lose::~Dialog_win_lose()
{
    delete ui;
}

void Dialog_win_lose::set_the_label(QString in)
{
    ui->label->setText(in);
}
