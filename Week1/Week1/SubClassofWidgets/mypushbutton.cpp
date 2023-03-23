#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent):QPushButton(parent), numClicks_(0)
{

}

void MyPushButton::incNumClicks() { ++ numClicks_; }
