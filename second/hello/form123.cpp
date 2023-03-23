#include "form123.h"
#include "ui_form123.h"

Form123::Form123(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form123)
{
    ui->setupUi(this);
}

Form123::~Form123()
{
    delete ui;
}

void Form123::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap(":/chessboard_small.png"));
}
