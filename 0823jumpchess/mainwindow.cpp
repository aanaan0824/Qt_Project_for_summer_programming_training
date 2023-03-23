#include "mainwindow.h"
//#include "ui_dialog_win_lose.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listenSocket(nullptr)
    , readWriteSocket(nullptr)
    , whether_a_server(0)
    , whether_after_click(0)
    , which_color_is_playing(2)
    , chessnum(0)
    , roundnum(0)
    , whether_start_the_game(0)
    , can_start_the_game(0)
    , first_start_timer(0)
{
    ui->setupUi(this);
    ui->widget->setVisible(0);
    this_player.overtime=0;
    this_player.stepnum=0;
    that_player.overtime=0;
    that_player.stepnum=0;
    this_player.win=0;
    that_player.win=0;
    mytimer=new QTimer(this);
    timernum=0;
    connect(mytimer,&QTimer::timeout, [=](){
        //static int timernum=0;
        if(timernum==21)
        {
            if(which_color_is_playing==this_player.color)
            {
                this_player.overtime++;
                this_player.stepnum++;
                if(this_player.overtime>=3)
                {
                    mytimer->stop();
                    lose_pop_up();
                    return;
                }
                else ui->label_timeout_you->setText(QString("YOUR TIMEOUT TIMES: %1").arg(this_player.overtime));
            }
            else
            {
                that_player.overtime++;
                that_player.stepnum++;
                if(that_player.overtime>=3)
                {
                    mytimer->stop();
                    win_pop_up();
                    return;
                }
                else ui->label_timeout_rival->setText(QString("RIVAL'S TIMEOUT TIMES: %1").arg(that_player.overtime));
            }
            roundnum=(this_player.stepnum+that_player.stepnum)/2+1;
            ui->label_round->setText(QString("ROUND: %1").arg(roundnum));
            //this_player.stepnum++;
            //that_player.stepnum++;
            whether_after_click=0;
            timernum=0;
            which_color_is_playing=which_color_is_playing%2+1;
            mytimer->setInterval(1000);
            mytimer->start();
            if(this_player.color==which_color_is_playing) ui->label_whose_turn->setText("YOUR TURN");
            else ui->label_whose_turn->setText("NOT YOUR TURN");
        }
        else{
            timernum++;
            ui->lcdNumber_time->display(21-timernum);
        }
    });

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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionCreate_the_connection_triggered()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);

    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    QString servIP;
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            servIP = address.toString();
    }
    create=new Dialog_create_connection;
    create->set_the_label(servIP);
    create->show();
    connect(create, &Dialog_create_connection::rejected ,[=](){
        delete listenSocket;
        return;
    });
    connect(create, &Dialog_create_connection::accepted ,[=](){
        qDebug()<<"connected good";
        this_player.color=2;
        that_player.color=1;
        whether_a_server=1;
        QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
        ui->label_whose_turn->setText("YOUR TURN");
    });
}

void MainWindow::acceptConnection()
{
    this->readWriteSocket =this->listenSocket->nextPendingConnection();
    can_start_the_game=1;
    ui->widget->setVisible(1);
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void MainWindow::on_actionConnect_to_Server_triggered()
{
    connectto=new Dialog_connect_to_server;
    connectto->show();
    connect(connectto, &Dialog_connect_to_server::accepted ,[=](){
        QRegularExpression regexp("^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\.(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$");
        Serverip=connectto->getServerIP();
        bool ismatch=regexp.match(Serverip).hasMatch();
        if(ismatch==0)
        {
            result=new Dialog_win_lose;
            result->set_the_label("Illegal IP!");
            result->show();
            return;
        }
        qDebug()<<"connecting good";
        can_start_the_game=1;
        ui->widget->setVisible(1);
        this_player.color=1;
        that_player.color=2;
        this->readWriteSocket = new QTcpSocket;
        this->readWriteSocket->connectToHost(QHostAddress(Serverip),8888);
        QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
        ui->label_whose_turn->setText("NOT YOUR TURN");
    });
    connect(connectto, &Dialog_connect_to_server::rejected ,[=](){
        return;
    });
}

void MainWindow::recvMessage()
{
    /*QString info;
    info += this->readWriteSocket->readAll();
    qDebug()<<info;*/
    if(first_start_timer==0){
        timernum=0;
        mytimer->setInterval(1000);
        mytimer->start();
        first_start_timer=1;
        QString info;
        info = this->readWriteSocket->readAll();
        return;
    }
    QString info;
    info += this->readWriteSocket->readAll();
    info=info.mid(0,info.indexOf('}')+1);
    qDebug()<<info;
    QJsonDocument jdoc = QJsonDocument::fromJson(info.toUtf8());
    QJsonObject jobject = jdoc.object();
    //qDebug()<<jobject;
    int floatr=jobject.value("floatr").toInt();
    qDebug()<<floatr;
    int floatq=jobject.value("floatq").toInt();
    qDebug()<<floatq;
    int clickr=jobject.value("clickr").toInt();
    qDebug()<<clickr;
    int clickq=jobject.value("clickq").toInt();
    qDebug()<<clickq;
    qDebug()<<floatr<<floatq<<clickr<<clickq;

    //if rival is in a special situation
    if(floatr==0 && floatq==0 && clickq==0 && clickr==0)
    {
        return;
    }
    if(floatr==666)
    {
        mytimer->stop();
        win_pop_up();
        return;
    }

    thisboard[clickr][clickq].chesscolor=thisboard[floatr][floatq].chesscolor;
    thisboard[floatr][floatq].chesscolor=0;
    repaint(thisboard[clickr][clickq].x,thisboard[clickr][clickq].y,32,32);
    repaint(thisboard[floatr][floatq].x,thisboard[floatr][floatq].y,32,32);
    that_player.stepnum++;
    roundnum=(this_player.stepnum+that_player.stepnum)/2+1;
    ui->label_round->setText(QString("ROUND: %1").arg(roundnum));
    ui->label_whose_turn->setText("YOUR TURN");
    timernum=0;
    mytimer->setInterval(1000);
    mytimer->start();
    which_color_is_playing=which_color_is_playing%2+1;
    whether_win();
}

//former void&func

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
        drawchess(620,305,this_player.color);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(whether_start_the_game==1){
        if(whether_after_click==0)
        {
            MainWindow::pos clickone=click_a_point(ev->pos().x(),ev->pos().y());
            if(clickone.chesscolor!=which_color_is_playing || which_color_is_playing!=this_player.color)return;
            if(first_start_timer==0){
                first_start_timer=1;
                mytimer->start(1000);
                QString sendtxt = "^^";
                QByteArray *array=new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
            }
            thisboard[clickone.row][clickone.queue].chesscolor=3;
            repaint(clickone.x,clickone.y,32,32);
            the_chess_float=clickone;
            //qDebug()<<clickone.x<<clickone.y<<clickone.chesscolor;
            //repaint();
            whether_after_click=1;
        }
        else if(whether_after_click==1)
        {
            MainWindow::pos clickone=click_a_point(ev->pos().x(),ev->pos().y());
            if(clickone.row==the_chess_float.row && clickone.queue==the_chess_float.queue)
            {
                thisboard[clickone.row][clickone.queue].chesscolor=the_chess_float.chesscolor;
                whether_after_click=0;
                //which_color_is_playing=which_color_is_playing%2+1;
                repaint(clickone.x,clickone.y,32,32);
                return;
            }
            if(clickone.chesscolor!=0)return;
            if(!can_this_chess_be_there(the_chess_float,clickone))return;
            thisboard[the_chess_float.row][the_chess_float.queue].chesscolor=0;
            thisboard[clickone.row][clickone.queue].chesscolor=the_chess_float.chesscolor;
            whether_after_click=0;
            which_color_is_playing=which_color_is_playing%2+1;
            repaint(clickone.x,clickone.y,32,32);
            repaint(the_chess_float.x,the_chess_float.y,32,32);
            this_player.stepnum++;
            roundnum=(this_player.stepnum+that_player.stepnum)/2+1;
            ui->label_round->setText(QString("ROUND: %1").arg(roundnum));
            ui->label_whose_turn->setText("NOT YOUR TURN");
            timernum=0;
            mytimer->setInterval(1000);
            mytimer->start();
            whether_win();
            QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(the_chess_float.row).arg(the_chess_float.queue).arg(clickone.row).arg(clickone.queue);
            qDebug()<<sendtxt;
            QByteArray *array =new QByteArray;
            array->clear();
            array->append(sendtxt.toUtf8());
            readWriteSocket->write(array->data());
        }
        else QMainWindow::mousePressEvent(ev);
    }
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

void MainWindow::on_actionStart_triggered()
{
    if(can_start_the_game){
        whether_start_the_game=1;
        repaint();
    }
}

void MainWindow::win_pop_up()
{
    result = new Dialog_win_lose;
    result->set_the_label("You Win!");
    result->show();
}

void MainWindow::lose_pop_up()
{
    result = new Dialog_win_lose;
    result->set_the_label("You Lose!");
    result->show();
}

int MainWindow::whether_win()
{
    if(this_player.stepnum==20)
    {
        if(this_player.color==1)
        {
            int thisnum=eq(1,thisboard[0][6].chesscolor)+eq(1,thisboard[1][6].chesscolor)+eq(1,thisboard[1][7].chesscolor)+eq(1,thisboard[2][6].chesscolor)+eq(1,thisboard[2][5].chesscolor)+eq(1,thisboard[2][7].chesscolor)+eq(1,thisboard[3][6].chesscolor)+eq(1,thisboard[3][5].chesscolor)+eq(1,thisboard[3][7].chesscolor)+eq(1,thisboard[3][8].chesscolor);
            if(thisnum>5){
                mytimer->stop();
                lose_pop_up();
                QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
                QByteArray *array =new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
                return -1;
            }
        }
        else
        {
            int thisnum=eq(2,thisboard[16][6].chesscolor)+eq(2,thisboard[15][6].chesscolor)+eq(2,thisboard[15][7].chesscolor)+eq(2,thisboard[14][6].chesscolor)+eq(2,thisboard[14][5].chesscolor)+eq(2,thisboard[14][7].chesscolor)+eq(2,thisboard[13][6].chesscolor)+eq(2,thisboard[13][5].chesscolor)+eq(2,thisboard[13][7].chesscolor)+eq(2,thisboard[13][8].chesscolor);
            if(thisnum>5){
                mytimer->stop();
                lose_pop_up();
                QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
                QByteArray *array =new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
                return -1;
            }
        }
    }
    else if (this_player.stepnum==25)
    {
        if(this_player.color==1)
        {
            int thisnum=eq(1,thisboard[0][6].chesscolor)+eq(1,thisboard[1][6].chesscolor)+eq(1,thisboard[1][7].chesscolor)+eq(1,thisboard[2][6].chesscolor)+eq(1,thisboard[2][5].chesscolor)+eq(1,thisboard[2][7].chesscolor)+eq(1,thisboard[3][6].chesscolor)+eq(1,thisboard[3][5].chesscolor)+eq(1,thisboard[3][7].chesscolor)+eq(1,thisboard[3][8].chesscolor);
            if(thisnum>2){
                mytimer->stop();
                lose_pop_up();
                QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
                QByteArray *array =new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
                return -1;
            }
        }
        else
        {
            int thisnum=eq(2,thisboard[16][6].chesscolor)+eq(2,thisboard[15][6].chesscolor)+eq(2,thisboard[15][7].chesscolor)+eq(2,thisboard[14][6].chesscolor)+eq(2,thisboard[14][5].chesscolor)+eq(2,thisboard[14][7].chesscolor)+eq(2,thisboard[13][6].chesscolor)+eq(2,thisboard[13][5].chesscolor)+eq(2,thisboard[13][7].chesscolor)+eq(2,thisboard[13][8].chesscolor);
            if(thisnum>2){
                mytimer->stop();
                lose_pop_up();
                QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
                QByteArray *array =new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
                return -1;
            }
        }
    }
    else if (this_player.stepnum==30)
    {
        if(this_player.color==1)
        {
            int thisnum=eq(1,thisboard[0][6].chesscolor)+eq(1,thisboard[1][6].chesscolor)+eq(1,thisboard[1][7].chesscolor)+eq(1,thisboard[2][6].chesscolor)+eq(1,thisboard[2][5].chesscolor)+eq(1,thisboard[2][7].chesscolor)+eq(1,thisboard[3][6].chesscolor)+eq(1,thisboard[3][5].chesscolor)+eq(1,thisboard[3][7].chesscolor)+eq(1,thisboard[3][8].chesscolor);
            if(thisnum>0){
                mytimer->stop();
                lose_pop_up();
                QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
                QByteArray *array =new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
                return -1;
            }
        }
        else
        {
            int thisnum=eq(2,thisboard[16][6].chesscolor)+eq(2,thisboard[15][6].chesscolor)+eq(2,thisboard[15][7].chesscolor)+eq(2,thisboard[14][6].chesscolor)+eq(2,thisboard[14][5].chesscolor)+eq(2,thisboard[14][7].chesscolor)+eq(2,thisboard[13][6].chesscolor)+eq(2,thisboard[13][5].chesscolor)+eq(2,thisboard[13][7].chesscolor)+eq(2,thisboard[13][8].chesscolor);
            if(thisnum>0){
                mytimer->stop();
                lose_pop_up();
                QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
                QByteArray *array =new QByteArray;
                array->clear();
                array->append(sendtxt.toUtf8());
                readWriteSocket->write(array->data());
                return -1;
            }
        }
    }
    int num2=eq(2,thisboard[0][6].chesscolor)+eq(2,thisboard[1][6].chesscolor)+eq(2,thisboard[1][7].chesscolor)+eq(2,thisboard[2][6].chesscolor)+eq(2,thisboard[2][5].chesscolor)+eq(2,thisboard[2][7].chesscolor)+eq(2,thisboard[3][6].chesscolor)+eq(2,thisboard[3][5].chesscolor)+eq(2,thisboard[3][7].chesscolor)+eq(2,thisboard[3][8].chesscolor);
    int num1=eq(1,thisboard[16][6].chesscolor)+eq(1,thisboard[15][6].chesscolor)+eq(1,thisboard[15][7].chesscolor)+eq(1,thisboard[14][6].chesscolor)+eq(1,thisboard[14][5].chesscolor)+eq(1,thisboard[14][7].chesscolor)+eq(1,thisboard[13][6].chesscolor)+eq(1,thisboard[13][5].chesscolor)+eq(1,thisboard[13][7].chesscolor)+eq(1,thisboard[13][8].chesscolor);
    if(num1==10)
    {
        if(this_player.color==1)
        {
            mytimer->stop();
            win_pop_up();
            return 1;
        }
        else
        {
            mytimer->stop();
            lose_pop_up();
            return -1;
        }
    }
    if(num2==10)
    {
        if(this_player.color==2)
        {
            mytimer->stop();
            win_pop_up();
            return 1;
        }
        else
        {
            mytimer->stop();
            lose_pop_up();
            return -1;
        }
    }
    return 0;
}

int MainWindow::eq(int x,int y)
{
    if(x==y)return 1;
    else return 0;
}

void MainWindow::on_actionAdmit_defeat_triggered()
{
    if(this_player.stepnum+that_player.stepnum>=20)
    {
        mytimer->stop();
        lose_pop_up();
        QString sendtxt = QString("{\"floatr\":%1,\"floatq\":%2,\"clickr\":%3,\"clickq\":%4}").arg(666).arg(666).arg(666).arg(666);
        QByteArray *array =new QByteArray;
        array->clear();
        array->append(sendtxt.toUtf8());
        readWriteSocket->write(array->data());
    }
}

