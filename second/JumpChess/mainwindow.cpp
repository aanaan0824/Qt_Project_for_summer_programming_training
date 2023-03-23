#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , bo(this)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(),Qt::black);
}

class MainWindow::player
{
public:
    player(int co):step(0),color(co){}
    ~player(){}
    int get_step()
    {
        return step;
    }
    int get_color()
    {
        return color;
    }
private:
    int step;
    int color;
protected:
};

/*void MainWindow::mousePressEvent(QMouseEvent* ev)
{
    ui->label_test->setText(QString("X:%1 Y:%2").arg(ev->pos().x()).arg(ev->pos().y()));
}*/


