#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include<QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT

public:
    MyPushButton(QWidget *parent);

    int getNumClicks() { return numClicks_; }

public slots:
    void incNumClicks();

protected:
    int numClicks_;
};

#endif // MYPUSHBUTTON_H
