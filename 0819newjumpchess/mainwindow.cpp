#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , whether_after_click(0)
    , which_color_is_playing(2)
    , chessnum(0)
    , roundnum(0)
    , whether_start_the_game(0)
{
    ui->setupUi(this);

    //set up the chessboard

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
            thisboard[i][j].row=i;
            thisboard[i][j].queue=j;
        }
    }

    //visible setting
    ui->label->setVisible(0);
    ui->lcdNumber->setVisible(0);

    //slot-signal
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintcircle(int x,int y)
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

void MainWindow::drawchess(int x, int y, int c)
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
    if(c!=3)pen.setColor(Qt::white);
    else pen.setColor(Qt::black);
    p.setPen(pen);
    p.setBrush(QBrush(Qt::NoBrush));
    p.drawEllipse(x+3,y+3,18,18);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(this->rect(),Qt::black);
    if(whether_start_the_game){
        for(int i=0;i<17;i++)
            for(int j=0;j<13;j++)
                drawchess(thisboard[i][j].x,thisboard[i][j].y,thisboard[i][j].chesscolor);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(whether_after_click==0)
    {
        MainWindow::pos clickone=click_a_point(ev->pos().x(),ev->pos().y());
        if(clickone.chesscolor!=which_color_is_playing)return;
        thisboard[clickone.row][clickone.queue].chesscolor=3;
        repaint(clickone.x,clickone.y,32,32);
        the_chess_float=clickone;
        //qDebug()<<clickone.x<<clickone.y<<clickone.chesscolor;
        //repaint();
        whether_after_click=1;
        which_color_is_playing=which_color_is_playing%2+1;
    }
    else if(whether_after_click==1)
    {
        MainWindow::pos clickone=click_a_point(ev->pos().x(),ev->pos().y());
        if(clickone.row==the_chess_float.row && clickone.queue==the_chess_float.queue)
        {
            thisboard[clickone.row][clickone.queue].chesscolor=the_chess_float.chesscolor;
            whether_after_click=0;
            which_color_is_playing=which_color_is_playing%2+1;
            repaint(clickone.x,clickone.y,32,32);
            return;
        }
        if(clickone.chesscolor!=0)return;
        if(!can_this_chess_be_there(the_chess_float,clickone))return;
        thisboard[the_chess_float.row][the_chess_float.queue].chesscolor=0;
        thisboard[clickone.row][clickone.queue].chesscolor=the_chess_float.chesscolor;
        whether_after_click=0;
        repaint(clickone.x,clickone.y,32,32);
        repaint(the_chess_float.x,the_chess_float.y,32,32);
    }
    else QMainWindow::mousePressEvent(ev);
}

MainWindow::pos MainWindow::click_a_point(int x,int y)
{
    MainWindow::pos defau{-1,-1,-1,-1,-1};
    int cx,cy;
    cx=x;
    cy=y;
    qDebug()<<x<<' '<<y;
    if(cx>=50 && cx<=50+13*32 && cy>=50 && cy<=106+28*15)
    {
        cx-=16;cy-=16;
        if((cy-50)%28<=12 || (cy-50)%28>=16)
        {
            int row=(cy-50+16)/28;
            int queue;
            if(row%2==0)
            {
                if((cx-50)%32<=12 || (cx-50)%32>=20)
                {
                    queue=(cx-50+16)/32;
                }
                else return defau;
            }
            else
            {
                if((cx-50+16)%32<=12 || (cx-50+16)%32>=20)
                {
                    queue=(cx-50)/32+1;
                }
                else return defau;
            }
            qDebug()<<row<<' '<<queue;
            return thisboard[row][queue];
        }
        else return defau;
    }
    else return defau;
}

bool MainWindow::can_this_chess_be_there(pos ori,pos next)
{
    if(next.chesscolor!=0)return false;
    if(are_you_neighbors(ori,next))return true;
    for(int i=0;i<17;i++){
        for(int j=0;j<13;j++){
            if(thisboard[i][j].chesscolor!=0 && thisboard[i][j].chesscolor!=3)canit[i][j]=-1;
            else canit[i][j]=0;
        }
    }
    dfs(ori.row,ori.queue,next.row,next.queue);
    if(canit[next.row][next.queue]==1)return true;
    return false;
}

void MainWindow::dfs(int nr,int nq,int fr,int fq)
{
    if(nr>16 || nr<0 || nq>13 || nq<0)return;
    if(canit[nr][nq]!=0)return;
    canit[nr][nq]=1;
    qDebug()<<nr<<nq<<fr<<fq;
    if(nr==fr && nq==fq)
    {
        return;
    }
    if(nq>0)
        if(thisboard[nr][nq-1].chesscolor==1 || thisboard[nr][nq-1].chesscolor==2)
            dfs(nr,nq-2,fr,fq);
    if(nq<12)
        if(thisboard[nr][nq+1].chesscolor==1 || thisboard[nr][nq+1].chesscolor==2)
            dfs(nr,nq+2,fr,fq);
    if(nr%2==0)
    {
        if(nr>0){
            if(thisboard[nr-1][nq].chesscolor==1 || thisboard[nr-1][nq].chesscolor==2)
                dfs(nr-2,nq-1,fr,fq);
            if(nq<12)
                if(thisboard[nr-1][nq+1].chesscolor==1 || thisboard[nr-1][nq+1].chesscolor==2)
                    dfs(nr-2,nq+1,fr,fq);
        }
        if(nr<16){
            if(thisboard[nr+1][nq].chesscolor==1 || thisboard[nr+1][nq].chesscolor==2)
                dfs(nr+2,nq-1,fr,fq);
            if(nq<12)
                if(thisboard[nr+1][nq+1].chesscolor==1 || thisboard[nr+1][nq+1].chesscolor==2)
                    dfs(nr+2,nq+1,fr,fq);
        }
    }
    else
    {
        if(nr>0){
            if(thisboard[nr-1][nq].chesscolor==1 || thisboard[nr-1][nq].chesscolor==2)
                dfs(nr-2,nq+1,fr,fq);
            if(nq>0)
                if(thisboard[nr-1][nq-1].chesscolor==1 || thisboard[nr-1][nq-1].chesscolor==2)
                    dfs(nr-2,nq-1,fr,fq);
        }
        if(nr<16){
            if(thisboard[nr+1][nq].chesscolor==1 || thisboard[nr+1][nq].chesscolor==2)
                dfs(nr+2,nq+1,fr,fq);
            if(nq>0)
                if(thisboard[nr+1][nq-1].chesscolor==1 || thisboard[nr+1][nq-1].chesscolor==2)
                    dfs(nr+2,nq-1,fr,fq);
        }
    }
}

bool MainWindow::are_you_neighbors(pos a,pos b)
{
    int ar=a.row;int aq=a.queue;
    int br=b.row;int bq=b.queue;
    if(ar-br>1 || ar-br<-1)return false;
    if(ar==br){
        if(aq-bq==1 || aq-bq==-1)
            return true;
        else return false;
    }
    if(ar%2==0)
    {
        if(bq==aq || bq==aq+1)return true;
    }
    else
    {
        if(bq==aq-1 || bq==aq)return true;
    }
    return false;
}

MainWindow::pos MainWindow::the_next_pos(pos ori,pos next)
{
    if(ori.row==next.row)
    {
        return thisboard[ori.row][next.queue*2-ori.queue];
    }
    else
    {
        if(ori.queue!=next.queue)
        {
            return thisboard[next.row*2-ori.row][next.queue];
        }
        else
        {
            if(ori.row%2==0)
            {
                return thisboard[next.row*2-ori.row][next.queue-1];
            }
            else
            {
                return thisboard[next.row*2-ori.row][next.queue+1];
            }
        }
    }
}

/*void timerEvent(QTimerEvent *event)
{
    QTimer timer;
    //timer.
}*/

