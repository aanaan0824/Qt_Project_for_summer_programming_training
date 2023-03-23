#include "dialog.h"
#include "ui_dialog.h"
#include <QRandomGenerator64>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
{
    qint64 num = qAbs(QRandomGenerator64::global()->generate() & std::numeric_limits<qint64>::max());
    qDebug("num: %d\n", num);
    QString numStr;
    numStr.setNum(num%5+1);
    qDebug("numStr: %d\n", numStr.toInt());
    QString str = ":/new/picture/pictures/secondGate";
    str += numStr;
    str += ".jpg";
    QPixmap pm(str);
    ui->label->setPixmap(pm);
}
