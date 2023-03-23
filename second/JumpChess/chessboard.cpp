#include "chessboard.h"
#include "ui_chessboard.h"

chessboard::chessboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chessboard)
    , whether_after_click(0)
{
    ui->setupUi(this);
    for(int i=0;i<17;i++)
        for(int j=0;j<13;j++)
            thisboard[i][j].chesscolor=-1;
    thisboard[0][6].chesscolor=1;
    for(int i=6;i<8;i++)thisboard[1][i].chesscolor=1;
    for(int i=5;i<9;i++)thisboard[3][i].chesscolor=1;
    for(int i=5;i<8;i++)thisboard[2][i].chesscolor=1;
    thisboard[16][6].chesscolor=2;
    for(int i=6;i<8;i++)thisboard[15][i].chesscolor=2;
    for(int i=5;i<9;i++)thisboard[13][i].chesscolor=2;
    for(int i=5;i<8;i++)thisboard[14][i].chesscolor=2;
    for(int i=0;i<13;i++)thisboard[4][i].chesscolor=0;
    for(int i=1;i<13;i++)thisboard[5][i].chesscolor=0;
    for(int i=1;i<12;i++)thisboard[6][i].chesscolor=0;
    for(int i=2;i<12;i++)thisboard[7][i].chesscolor=0;
    for(int i=2;i<11;i++)thisboard[8][i].chesscolor=0;
    for(int i=0;i<13;i++)thisboard[12][i].chesscolor=0;
    for(int i=1;i<13;i++)thisboard[11][i].chesscolor=0;
    for(int i=1;i<12;i++)thisboard[10][i].chesscolor=0;
    for(int i=2;i<12;i++)thisboard[9][i].chesscolor=0;
    for(int i=0;i<17;i++){
        for(int j=0;j<13;j++){
            if(i%2==0){
                thisboard[i][j].x=50+j*32;
            }
            else{
                thisboard[i][j].x=50-16+j*32;
            }
            thisboard[i][j].y=50+i*28;
        }
    }
}

chessboard::~chessboard()
{
    delete ui;
}

void chessboard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(),Qt::black);
    for(int i=0;i<17;i++)
        for(int j=0;j<13;j++)
            drawchess(thisboard[i][j].x,thisboard[i][j].y,thisboard[i][j].chesscolor);
}

void chessboard::paintcircle(int x,int y)
{
    QPainter p(this);

    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(1);
    p.setPen(pen);

    p.drawEllipse(x,y,24,24);
    pen.setWidth(2);
    p.setPen(pen);
    p.drawPoint(x+12,y+12);
}

void chessboard::drawchess(int x, int y, int c)
{
    QPainter p(this);

    QPen pen;
    QBrush brush;
    if(c==0)
    {
        paintcircle(x, y);
        return;
    }
    if(c==-1)
    {
        return;
    }
    if(c==1)
    {
        pen.setColor(qRgb(255,0,132));
        brush.setColor(qRgb(255,0,132));
    }
    else if(c==2)
    {
        pen.setColor(qRgb(0,255,242));
        brush.setColor(qRgb(0,255,242));
    }
    else if(c==3)
    {
        pen.setColor(qRgb(255,255,255));
        brush.setColor(qRgb(255,255,255));
    }
    brush.setStyle(Qt::SolidPattern);
    //pen.setColor(Qt::white);
    pen.setWidth(1);
    p.setPen(pen);
    p.setBrush(brush);
    p.drawEllipse(x,y,24,24);
    pen.setWidth(1);
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.setBrush(QBrush(Qt::NoBrush));
    p.drawEllipse(x+3,y+3,18,18);
}

/*void chessboard::mouseMoveEvent(QMouseEvent *ev)
{
    if(whether_after_click==1){
        drawchess(ev->pos().x()-16,ev->pos().y()-16,1);
    }
}*/

void chessboard::mousePressEvent(QMouseEvent *ev)
{
    if(whether_after_click==0)
    {
        chessboard::pos clickone=click_a_point(ev->pos().x(),ev->pos().y());
        //if(clickone.chesscolor==-1 || clickone.chesscolor==0)return;
        thisboard[clickone.x][clickone.y].chesscolor=3;
        qDebug()<<clickone.x<<' '<<clickone.y;
    }
}

chessboard::pos chessboard::click_a_point(int x,int y)
{
    chessboard::pos defau{-1,-1,-1};
    int cx,cy;
    cx=x;
    cy=y;
    if(cx>=50 && cx<=50+13*32 && cy>=50 && cy<=106+28*15)
    {
        cx-=16;cy-=16;
        if((cy-50)%28<=8 || (cy-50)%28>=20)
        {
            int row=(cy-50+16)/28;
            int queue;
            if(row%2==0)
            {
                if((cx-50)%32<=8 || (cx-50)%32>=24)
                {
                    queue=(cx-50+16)/32;
                }
                else return defau;
            }
            else
            {
                if((cx-50+16)%32<=8 || (cx-50+16)%32>=24)
                {
                    queue=(cx-50+16+16)/32;
                }
                else return defau;
            }
            return thisboard[row][queue];
        }
        else return defau;
    }
    else return defau;
}
