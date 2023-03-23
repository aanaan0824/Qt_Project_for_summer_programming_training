#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, [=](){ emit keyPressed(7); });
    connect(ui->pushButton_2, &QPushButton::clicked, [=](){ emit keyPressed(8); });
    connect(ui->pushButton_3, &QPushButton::clicked, [=](){ emit keyPressed(9); });
    connect(ui->pushButton_4, &QPushButton::clicked, [=](){ emit keyPressed(4); });
    connect(ui->pushButton_5, &QPushButton::clicked, [=](){ emit keyPressed(5); });
    connect(ui->pushButton_6, &QPushButton::clicked, [=](){ emit keyPressed(6); });
    connect(ui->pushButton_7, &QPushButton::clicked, [=](){ emit keyPressed(1); });
    connect(ui->pushButton_8, &QPushButton::clicked, [=](){ emit keyPressed(2); });
    connect(ui->pushButton_9, &QPushButton::clicked, [=](){ emit keyPressed(3); });
    connect(ui->pushButton_10, &QPushButton::clicked, [=](){ emit keyPressed(0); });
    connect(this, &MainWindow::keyPressed, this, &MainWindow::getKeyValue);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getKeyValue(int num)
{
    qDebug("Pressed %d\n", num);
}
