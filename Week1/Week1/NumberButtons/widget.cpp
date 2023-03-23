#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->pushButton_0->setNum(0);
    ui->pushButton_1->setNum(1);
    ui->pushButton_2->setNum(2);
    ui->pushButton_3->setNum(3);
    ui->pushButton_4->setNum(4);
    ui->pushButton_5->setNum(5);
    ui->pushButton_6->setNum(6);
    ui->pushButton_7->setNum(7);
    ui->pushButton_8->setNum(8);
    ui->pushButton_9->setNum(9);

    QObject::connect(ui->pushButton_0, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_1, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_2, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_3, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_4, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_5, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_6, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_7, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_8, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(ui->pushButton_9, SIGNAL(clicked(int)), ui->lcdNumber, SLOT(display(int)));
}

Widget::~Widget()
{
    delete ui;
}

