#include "qintpushbutton.h"

QIntPushButton::QIntPushButton(QWidget *parent)
{
    num = 0;
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(click()));
}

void QIntPushButton::setNum(int n)
{
    num = n;
}

void QIntPushButton::click()
{
    emit clicked(num);
}
