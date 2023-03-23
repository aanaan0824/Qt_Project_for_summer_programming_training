#include "dialog2.h"
#include "ui_dialog2.h"
#include <iostream>
#include <regex>

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
}

Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::on_buttonBox_accepted()
{
    QString s=ui->lineEdit->text();
    if(!isValidInput(s)){
        QMessageBox::warning(0,"Warning","非法的IP地址！请重新输入。",QMessageBox::Ok);
        reject();
    }
    else{
        received=1;
        q=s;
        accept();
    }
}
bool Dialog2::isValidInput(QString s){
    std::regex r("^(([0-9]\\.)|([1-9][0-9]\\.)|(1[0-9][0-9]\\.)|(2[0-4][0-9]\\.)|(25[0-5]\\.)){3}(([0-9])|([1-9][0-9])|(1[0-9][0-9])|(2[0-4][0-9])|(25[0-5]))");
    if(regex_match(s.toStdString(),r)){
        return true;
    }
    return false;
}
