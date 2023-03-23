#ifndef QINTPUSHBUTTON_H
#define QINTPUSHBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>

class QIntPushButton : public QPushButton
{
    Q_OBJECT
public:
    QIntPushButton(QWidget *parent);
    void setNum(int n);
signals:
    void clicked(int);
public slots:
    void click();
private:
    int num;
};

#endif // QINTPUSHBUTTON_H
