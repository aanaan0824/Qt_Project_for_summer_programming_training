#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QMouseEvent>
#include <queue>
#include <QTest>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(694,480);
    for(int i=0;i<17;i++){
        for(int j=0;j<14;j++){
            _chess[i][j]=nullptr;
        }
    }
    //黄色棋子
    for(int i=0;i<4;i++){
        for(int j=0;j<i+1;j++){
            _chess[i][j]=new Chess(2,benchmark_x-i*12+j*24+36,benchmark_y+i*21,1,0);
            _chess[i][j]->arrayInfo.first=i;
            _chess[i][j]->arrayInfo.second=j;
        }
    }
    //空白棋盘
    for(int i=0;i<13;i++){
        _chess[4][i]=new Chess(benchmark_x-24*4-12+i*24,benchmark_y+4*21);
        _chess[4][i]->arrayInfo.first=4;
        _chess[4][i]->arrayInfo.second=i;
    }
    for(int i=0;i<12;i++){
        _chess[5][i]=new Chess(benchmark_x-24*4+i*24,benchmark_y+5*21);
        _chess[5][i]->arrayInfo.first=5;
        _chess[5][i]->arrayInfo.second=i;
    }
    for(int i=0;i<11;i++){
        _chess[6][i]=new Chess(benchmark_x-24*4+12+i*24,benchmark_y+6*21);
        _chess[6][i]->arrayInfo.first=6;
        _chess[6][i]->arrayInfo.second=i;
    }
    for(int i=0;i<10;i++){
        _chess[7][i]=new Chess(benchmark_x-24*3+i*24,benchmark_y+7*21);
        _chess[7][i]->arrayInfo.first=7;
        _chess[7][i]->arrayInfo.second=i;
    }
    for(int i=0;i<9;i++){
        _chess[8][i]=new Chess(benchmark_x-24*3+12+i*24,benchmark_y+8*21);
        _chess[8][i]->arrayInfo.first=8;
        _chess[8][i]->arrayInfo.second=i;
    }
    for(int i=0;i<10;i++){
        _chess[9][i]=new Chess(benchmark_x-24*3+i*24,benchmark_y+9*21);
        _chess[9][i]->arrayInfo.first=9;
        _chess[9][i]->arrayInfo.second=i;
    }
    for(int i=0;i<11;i++){
        _chess[10][i]=new Chess(benchmark_x-24*4+12+i*24,benchmark_y+10*21);
        _chess[10][i]->arrayInfo.first=10;
        _chess[10][i]->arrayInfo.second=i;
    }
    for(int i=0;i<12;i++){
        _chess[11][i]=new Chess(benchmark_x-24*4+i*24,benchmark_y+11*21);
        _chess[11][i]->arrayInfo.first=11;
        _chess[11][i]->arrayInfo.second=i;
    }
    for(int i=0;i<13;i++){
        _chess[12][i]=new Chess(benchmark_x-24*4-12+i*24,benchmark_y+12*21);
        _chess[12][i]->arrayInfo.first=12;
        _chess[12][i]->arrayInfo.second=i;
    }
    //蓝色棋子
    for(int i=0;i<4;i++){
        int k=0;
        for(int j=3-i;j>=0;j--){
            _chess[13+i][k]=new Chess(1,benchmark_x-3*12+k*24+i*12+36,benchmark_y+85*3+18+i*21,1,0);
            _chess[13+i][k]->arrayInfo.first=13+i;
            _chess[13+i][k]->arrayInfo.second=k;
            k++;
        }
    }
    //timer
    timer=new QTimer(this);
    timer->setInterval(50);
    //ui->lcdNumber->display(timecount);
    connect(timer,&QTimer::timeout,this,[=]()mutable{
        if(timecount>=0){
            cout<<"couting "<<timecount<<endl;
            ui->lcdNumber->display(timecount);
            timecount--;
        }
        else{
           timer->stop();
           if(currentPlayerType==myType){
               outOfTime++;
               updateInfo();
               QByteArray *array=new QByteArray;
               QString str="{\"f1\":-3,\"s1\":0,\"f2\":0,\"s2\":0}";
               array->clear();
               array->append(str.toStdString());
               this->socket->write(array->data());
               flag++;
               if(flag==2){
                   round++;
                   flag=0;
               }
               if(currentPlayerType==1){
                   currentPlayerType=2;
               }
               else currentPlayerType=1;
               judgement();
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i=0;i<17;i++){
        for(int j=0;j<14;j++){
            if(_chess[i][j]!=nullptr){
                delete _chess[i][j];
                _chess[i][j]=nullptr;
            }
        }
    }
    if(timer){
        if(timer->isActive()){
            timer->stop();
        }
        delete timer;
        timer=nullptr;
    }
}

void MainWindow::on_pushButton_clicked()
{
    //dlg1=new Dialog1;
    initServer();
    //dlg1->show();
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    QString servIP;
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            servIP = address.toString();
    }
    QMessageBox::information(0,"successfully connected",servIP,QMessageBox::Ok);
}
void MainWindow::on_pushButton_2_clicked()
{
    dlg2=new Dialog2;
    dlg2->show();
}
void MainWindow::on_pushButton_3_clicked()
{
    if(!isServer){
        if(dlg2->received)
            connectHost(dlg2->q);
        if(start==false){
            start=true;
            round=1;
            if(isServer){
                myType=1;
            }
            else{
                myType=2;
            }
        }
        updateInfo();
        QByteArray *array=new QByteArray;
        QString str="{\"f1\":-2,\"s1\":0,\"f2\":0,\"s2\":0}";
        array->clear();
        array->append(str.toStdString());
        this->socket->write(array->data());
        ui->lcdNumber->display(timecount);
        timer->start(1000);
    }
}
void MainWindow::on_pushButton_4_clicked()
{
    if(start&&round>=20){
        lose=1;
        QByteArray *array=new QByteArray;
        QString str="{\"f1\":-4,\"s1\":0,\"f2\":0,\"s2\":0}";
        array->clear();
        array->append(str.toStdString());
        this->socket->write(array->data());
        winOrLose();
    }
    else{
        QMessageBox::warning(0,"Warning","未到投降时间!",QMessageBox::Ok);
    }
}

void MainWindow::acceptConnection(){
    this->socket=this->server->nextPendingConnection();
    QObject::connect(this->socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
}
void MainWindow::assignMsg(int f1,int s1,int f2,int s2){
    if(f1>=0){
        locatePlayerChess.first=f1;
        locatePlayerChess.second=s1;
        locateChessBoard.first=f2;
        locateChessBoard.second=s2;
        move();
        updateInfo();
    }
    else if(f1==-4){
        //对面认输或判输
        win=1;
        start=false;
        winOrLose();
    }
    else if(f1==-1){
        //对面判赢
        lose=1;
        start=false;
        winOrLose();
    }
    else if(f1==-2){
        //对面准备
        start=true;
        round=1;
        myType=1;
        updateInfo();
        update();
        ui->lcdNumber->display(timecount);
        timer->start(1000);
    }
    else if(f1==-3){
        //对面超时
        flag++;
        if(flag==2){
            round++;
            flag=0;
        }
        if(currentPlayerType==1){
            currentPlayerType=2;
        }
        else currentPlayerType=1;
        updateInfo();
        timecount=20;
        timer->start(1000);
    }
}
void MainWindow::receiveMessage(){
    cout<<"msg received"<<endl;
    QByteArray info=this->socket->readAll();
    QString ss=info.data();
    if(ss.length()>50){
        QStringList list=ss.split('}');
        for(int i=0;i<2;i++){
            QString sss=list[i].append('}');
            cout<<sss.toStdString()<<endl;
            QJsonDocument jdoc=QJsonDocument::fromJson(sss.toUtf8());
            QJsonObject jobject=jdoc.object();
            int f1=jobject.value("f1").toInt();
            int s1=jobject.value("s1").toInt();
            int f2=jobject.value("f2").toInt();
            int s2=jobject.value("s2").toInt();
            cout<<"f1:"<<f1<<endl;
            assignMsg(f1,s1,f2,s2);
        }
    }
    else{
        QJsonDocument jdoc=QJsonDocument::fromJson(ss.toUtf8());
        QJsonObject jobject=jdoc.object();
        int f1=jobject.value("f1").toInt();
        int s1=jobject.value("s1").toInt();
        int f2=jobject.value("f2").toInt();
        int s2=jobject.value("s2").toInt();
        cout<<"f1:"<<f1<<endl;
        assignMsg(f1,s1,f2,s2);
    }
}
void MainWindow::initServer(){
    cout<<"init"<<endl;
    isServer=true;
    this->server=new QTcpServer;
    this->server->listen(QHostAddress::Any,8211);
    QObject::connect(this->server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

}
void MainWindow::connectHost(QString qst){
    cout<<"connected"<<endl;
    this->socket=new QTcpSocket;
    this->socket->connectToHost(QHostAddress(qst),8211);
    QObject::connect(this->socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));

}
void MainWindow::updateInfo(){
    if(myType==1){
        ui->label_3->setText("");
        ui->label_3->setStyleSheet("QLabel{background-color:rgb(135,206,235);}");
        ui->label_3->setText("蓝色");
    }
    else{
        ui->label_3->setText("");
        ui->label_3->setStyleSheet("QLabel{background-color:rgb(253,233,134);}");
        ui->label_3->setText("黄色");
    }
    ui->label_3->show();
    if(currentPlayerType==1){
        ui->label_5->setText("");
        ui->label_5->setStyleSheet("QLabel{background-color:rgb(135,206,235);}");
        ui->label_5->setText("蓝色");
    }
    else{
        ui->label_5->setText("");
        ui->label_5->setStyleSheet("QLabel{background-color:rgb(253,233,134);}");
        ui->label_5->setText("黄色");
    }
    ui->label_5->show();
    ui->label_7->setText(QString("%1").arg(outOfTime));
    ui->label_7->show();
    ui->label_9->setText(QString("%1").arg(round));
    ui->label_9->show();
}
void MainWindow::judgement(){
    //第20、25、30 回合时，需要判断离开阵营的棋子数是否达到 5、 8、 10颗。
    if((round==20&&own>5)||(round==25&&own>2)||(round==30&&own>0)){
        cout<<"judgement lose"<<endl;
        lose=1;
        QByteArray *array=new QByteArray;
        QString str="{\"f1\":-4,\"s1\":0,\"f2\":0,\"s2\":0}";
        array->clear();
        array->append(str.toStdString());
        this->socket->write(array->data());
        QCoreApplication::processEvents();
        winOrLose();
    }
    if(outOfTime==3){
        cout<<"judgement out of time"<<endl;
        lose=1;
        QByteArray *array=new QByteArray;
        QString str="{\"f1\":-4,\"s1\":0,\"f2\":0,\"s2\":0}";
        array->clear();
        array->append(str.toStdString());
        this->socket->write(array->data());
        QCoreApplication::processEvents();
        winOrLose();
    }
    if(opposite==10){
        win=1;
        QByteArray *array=new QByteArray;
        QString str="{\"f1\":-1,\"s1\":0,\"f2\":0,\"s2\":0}";
        array->clear();
        array->append(str.toStdString());
        this->socket->write(array->data());
        QCoreApplication::processEvents();
        winOrLose();
    }
}
void MainWindow::winOrLose(){
    if(win==1){
        timer->stop();
        start=false;
        QMessageBox::information(0,"Congratulations","你赢了！",QMessageBox::Ok);
    }
    else if(lose==1){
        timer->stop();
        start=false;
        QMessageBox::information(0,"Announcement","你输了!",QMessageBox::Ok);
    }
    else return;
}

void MainWindow::paintChess(QPaintEvent *ev,Chess *c){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::black);
    if(!c->getType()){
        p.setBrush(Qt::white);
    }
    else if(c->getType()==1){
        QColor skyblue(135,206,235);
        p.setBrush(skyblue);
    }
    else{
        QColor lightyellow(253,233,134);
        p.setBrush(lightyellow);
    }
    p.drawEllipse(c->getPosx(),c->getPosy(),20,20);
}
void MainWindow::paintEvent(QPaintEvent *ev){
    for(int i=0;i<17;i++){
        for(int j=0;j<13;j++){
            if(_chess[i][j]!=nullptr){
                paintChess(ev,_chess[i][j]);
                //QCoreApplication::processEvents();
            }
        }
    }
}

Chess* MainWindow::linkTo(int x,int y){
    for(int i=0;i<17;i++){
        for(int j=0;j<14;j++){
            if(_chess[i][j]==nullptr){
                continue;
            }
            if((pow(x-(_chess[i][j]->pos_x),2)+pow(y-(_chess[i][j]->pos_y),2))<=100){
                QCoreApplication::processEvents();
                return _chess[i][j];
            }
        }
    }
    return nullptr;
}
Chess* MainWindow::leftUp(Chess* c){
    if(c==nullptr){
        return nullptr;
    }
    return linkTo(c->pos_x-10,c->pos_y-21);
}
Chess* MainWindow::rightUp(Chess* c){
    if(c==nullptr){
        return nullptr;
    }
    return linkTo(c->pos_x+10,c->pos_y-21);
}
Chess* MainWindow::left(Chess* c){
    if(c==nullptr){
        return nullptr;
    }
    return linkTo(c->pos_x-20,c->pos_y);
}
Chess* MainWindow::right(Chess* c){
    if(c==nullptr){
        return nullptr;
    }
    return linkTo(c->pos_x+20,c->pos_y);
}
Chess* MainWindow::leftDown(Chess* c){
    if(c==nullptr){
        return nullptr;
    }
    return linkTo(c->pos_x-10,c->pos_y+21);
}
Chess* MainWindow::rightDown(Chess* c){
    if(c==nullptr){
        return nullptr;
    }
    return linkTo(c->pos_x+10,c->pos_y+21);
}
void MainWindow::chessWalk(){
    int f=locatePlayerChess.first;
    int s=locatePlayerChess.second;
    if(leftUp(_chess[f][s])!=nullptr){
        if(leftUp(_chess[f][s])->getType()==0){
            dfs[leftUp(_chess[f][s])->arrayInfo.first][leftUp(_chess[f][s])->arrayInfo.second]=1;
        }
    }
    if(left(_chess[f][s])!=nullptr){
        if(left(_chess[f][s])->getType()==0){
            dfs[left(_chess[f][s])->arrayInfo.first][left(_chess[f][s])->arrayInfo.second]=1;
        }
    }
    if(leftDown(_chess[f][s])!=nullptr){
        if(leftDown(_chess[f][s])->getType()==0){
            dfs[leftDown(_chess[f][s])->arrayInfo.first][leftDown(_chess[f][s])->arrayInfo.second]=1;
        }
    }
    if(rightUp(_chess[f][s])!=nullptr){
        if(rightUp(_chess[f][s])->getType()==0){
            dfs[rightUp(_chess[f][s])->arrayInfo.first][rightUp(_chess[f][s])->arrayInfo.second]=1;
        }
    }
    if(right(_chess[f][s])!=nullptr){
        if(right(_chess[f][s])->getType()==0){
            dfs[right(_chess[f][s])->arrayInfo.first][right(_chess[f][s])->arrayInfo.second]=1;
        }
    }
    if(rightDown(_chess[f][s])!=nullptr){
        if(rightDown(_chess[f][s])->getType()==0){
            dfs[rightDown(_chess[f][s])->arrayInfo.first][rightDown(_chess[f][s])->arrayInfo.second]=1;
        }
    }
    QCoreApplication::processEvents();
}
void MainWindow::chessJumpBfs(Chess *c, Chess *target){
    queue<Chess*> q;
    q.push(c);
    while(!q.empty()){
        pair<int,int> tmp;
        tmp.first=q.front()->arrayInfo.first;
        tmp.second=q.front()->arrayInfo.second;
        if(target->arrayInfo==tmp/*target->arrayInfo.first==q.front()->arrayInfo.first
                &&target->arrayInfo.second==q.front()->arrayInfo.second*/){
            dfs[target->arrayInfo.first][target->arrayInfo.second]=1;
            return;
        }
        if(q.front()->arrayInfo.first==0){
            if(_chess[1][0]->playerType!=0&&_chess[2][0]->playerType==0){
                dfs[2][0]=1;
                q.push(_chess[2][0]);
            }
            if(_chess[1][1]->playerType!=0&&_chess[2][2]->playerType==0){
                dfs[2][2]=1;
                q.push(_chess[2][2]);
            }
        }
        else if(q.front()->arrayInfo.first==16){
            if(_chess[15][0]->playerType!=0&&_chess[14][0]->playerType==0){
                dfs[14][0]=1;
                q.push(_chess[14][0]);
            }
            if(_chess[15][1]->playerType!=0&&_chess[14][2]->playerType==0){
                dfs[14][2]=1;
                q.push(_chess[14][2]);
            }
        }
        else if(q.front()->arrayInfo.first==1){
            if(q.front()->arrayInfo.second==0){
                if(_chess[2][0]->playerType!=0&&_chess[3][0]->playerType==0){
                    dfs[3][0]=1;
                    q.push(_chess[3][0]);
                }
                if(_chess[2][1]->playerType!=0&&_chess[3][2]->playerType==0){
                    dfs[3][2]=1;
                    q.push(_chess[3][2]);
                }
            }
            else{
                if(_chess[2][1]->playerType!=0&&_chess[3][1]->playerType==0){
                    dfs[3][1]=1;
                    q.push(_chess[3][1]);
                }
                if(_chess[2][2]->playerType!=0&&_chess[3][3]->playerType==0){
                    dfs[3][3]=1;
                    q.push(_chess[3][3]);
                }
            }
        }
        else if(q.front()->arrayInfo.first==15){
            if(q.front()->arrayInfo.second==0){
                if(_chess[14][0]->playerType!=0&&_chess[13][0]->playerType==0){
                    dfs[13][0]=1;
                    q.push(_chess[13][0]);
                }
                if(_chess[14][1]->playerType!=0&&_chess[13][2]->playerType==0){
                    dfs[13][2]=1;
                    q.push(_chess[13][2]);
                }
            }
            else{
                if(_chess[14][1]->playerType!=0&&_chess[13][1]->playerType==0){
                    dfs[13][1]=1;
                    q.push(_chess[13][1]);
                }
                if(_chess[14][2]->playerType!=0&&_chess[13][3]->playerType==0){
                    dfs[13][3]=1;
                    q.push(_chess[13][3]);
                }
            }
        }
        else{
            Chess* d=q.front();
            if(leftUp(d)!=nullptr){
                Chess* e=leftUp(d);
                if(leftUp(e)!=nullptr&&leftUp(d)->playerType!=0){
                    if(leftUp(e)->playerType==0
                            //&&dfs[leftUp(leftUp(d))->arrayInfo.first][leftUp(leftUp(d))->arrayInfo.second]!=1
                            ){
                        Chess* f=leftUp(e);
                        dfs[f->arrayInfo.first][f->arrayInfo.second]=1;
                        q.push(f);
                    }
                }
            }
            if(rightUp(d)!=nullptr){
                Chess* e=rightUp(d);
                if(rightUp(e)!=nullptr){
                    if(rightUp(d)->playerType!=0){
                        if(rightUp(e)->playerType==0&&rightUp(e)->arrayInfo.first!=-1
                                &&rightUp(e)->arrayInfo.second!=-1
                            //&&dfs[rightUp(rightUp(d))->arrayInfo.first][rightUp(rightUp(d))->arrayInfo.second]!=1
                                ){
                            Chess* f=rightUp(e);
                            dfs[f->arrayInfo.first][f->arrayInfo.second]=1;
                            q.push(f);
                        }
                    }
                }
            }
            if(left(d)!=nullptr){
                Chess* e=left(d);
                if(left(e)!=nullptr&&left(d)->playerType!=0){
                    if(left(e)->playerType==0
                            //&&dfs[left(left(d))->arrayInfo.first][left(left(d))->arrayInfo.second]!=1
                            ){
                        Chess* f=left(e);
                        dfs[f->arrayInfo.first][f->arrayInfo.second]=1;
                        q.push(f);
                    }
                }
            }
            if(right(d)!=nullptr){
                Chess* e=right(d);
                if(right(e)!=nullptr&&right(d)->playerType!=0){
                    if(right(e)->playerType==0
                        //&&dfs[right(right(d))->arrayInfo.first][right(right(d))->arrayInfo.second]!=1
                            ){
                        Chess* f=right(e);
                        dfs[f->arrayInfo.first][f->arrayInfo.second]=1;
                        q.push(f);
                    }
                }
            }
            if(leftDown(d)!=nullptr){
                Chess* e=leftDown(d);
                if(leftDown(e)!=nullptr){
                    if(e->playerType!=0){
                        if(leftDown(e)->playerType==0
                            //&&dfs[leftDown(leftDown(d))->arrayInfo.first][leftDown(leftDown(d))->arrayInfo.second]!=1
                                ){
                            Chess* f=leftDown(e);
                            dfs[f->arrayInfo.first][f->arrayInfo.second]=1;
                            q.push(f);
                        }
                    }
                }
            }
            if(rightDown(d)!=nullptr){
                Chess* e=rightDown(d);
                if(rightDown(e)!=nullptr){
                    if(rightDown(d)->playerType!=0){
                        if(rightDown(e)->playerType==0
                            //fs[rightDown(rightDown(d))->arrayInfo.first][rightDown(rightDown(d))->arrayInfo.second]!=1
                                ){
                            Chess* f=rightDown(e);
                            dfs[f->arrayInfo.first][f->arrayInfo.second]=1;
                            q.push(f);
                        }
                    }
                }
            }
        }
        QCoreApplication::processEvents();
        q.pop();
    }
    return;
}
void MainWindow::preMove(Chess *c){
    chessWalk();
    int f=locateChessBoard.first;
    int s=locateChessBoard.second;
    chessJumpBfs(c,_chess[f][s]);
}
void MainWindow::move(){
    timer->stop();
    timecount=20;
    timer->start(1000);

    int f1=locatePlayerChess.first;
    int s1=locatePlayerChess.second;
    int t=_chess[f1][s1]->getType();
    int x1=_chess[f1][s1]->getPosx();
    int y1=_chess[f1][s1]->getPosy();
    int ow=_chess[f1][s1]->checkedOwn;
    int op=_chess[f1][s1]->checkedOpposite;

    int f2=locateChessBoard.first;
    int s2=locateChessBoard.second;
    int x2=_chess[f2][s2]->getPosx();
    int y2=_chess[f2][s2]->getPosy();

    delete _chess[f1][s1];
    _chess[f1][s1]=nullptr;
    delete _chess[f2][s2];
    _chess[f2][s2]=nullptr;

    _chess[f2][s2]=new Chess(t,x2,y2,0,0);
    _chess[f2][s2]->arrayInfo.first=f2;
    _chess[f2][s2]->arrayInfo.second=s2;
    _chess[f2][s2]->checkedOwn=ow;
    _chess[f2][s2]->checkedOpposite=op;
    if(!_chess[f2][s2]->checkedOwn&&_chess[f2][s2]->getType()==myType){
        if(!_chess[f2][s2]->judgeOwn()){
            own--;
            _chess[f2][s2]->checkedOwn=true;
        }
    }
    else if(_chess[f2][s2]->checkedOwn&&_chess[f2][s2]->getType()==myType){
        if(_chess[f2][s2]->judgeOwn()){
            own++;
            _chess[f2][s2]->checkedOwn=false;
        }
    }
    if(!_chess[f2][s2]->checkedOpposite&&_chess[f2][s2]->getType()==myType){
        if(_chess[f2][s2]->judgeOpposite()){
            opposite++;
            _chess[f2][s2]->checkedOpposite=true;
        }
    }
    else if(_chess[f2][s2]->checkedOpposite&&_chess[f2][s2]->getType()==myType){
        if(!_chess[f2][s2]->judgeOpposite()){
            opposite--;
            _chess[f2][s2]->checkedOpposite=false;
        }
    }
    _chess[f1][s1]=new Chess(x1,y1);
    _chess[f1][s1]->arrayInfo.first=f1;
    _chess[f1][s1]->arrayInfo.second=s1;
    cout<<"move from "<<f1<<' '<<s1<<" to "<<f2<<' '<<s2<<endl;
    memset(dfs,0,sizeof(dfs));
    pieceChosen=false;
    placeChosen=false;
    flag++;
    if(flag==2){
        round++;
        flag=0;
    }
    if(currentPlayerType==1){
        currentPlayerType=2;
    }
    else currentPlayerType=1;
    QCoreApplication::processEvents();
    judgement();
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *e){
    int x=e->pos().x();
    int y=e->pos().y();
    cout<<start<<' '<<currentPlayerType<<' '<<myType<<endl;
    if(start&&currentPlayerType==myType){
        cout<<"im here"<<endl;
        if(e->button()==Qt::LeftButton&&x>170){
            for(int i=0;i<17;i++){
                for(int j=0;j<14;j++){
                    if(_chess[i][j]==nullptr) continue;
                    if(currentPlayerType==_chess[i][j]->getType()&&
                            (pow(x-(_chess[i][j]->getPosx()+10),2)+pow(y-(_chess[i][j]->getPosy()+10),2))<=100
                            /*&&locatePlayerChess.first==-1&&locatePlayerChess.second==-1*/){
                        placeChosen=false;
                        locatePlayerChess={i,j};
                        pieceChosen=true;
                        QCoreApplication::processEvents();
                    }
                    else if(!_chess[i][j]->getType()&&
                            (pow(x-(_chess[i][j]->getPosx()+10),2)+pow(y-(_chess[i][j]->getPosy()+10),2))<=100&&
                            /*locateChessBoard.first==-1&&locateChessBoard.second==-1&&*/pieceChosen==true){
                        locateChessBoard={i,j};
                        placeChosen=true;
                        int f=locatePlayerChess.first;
                        int s=locatePlayerChess.second;
                        preMove(_chess[f][s]);
                        QCoreApplication::processEvents();
                    }
                    else{
                        e->ignore();
                    }
                }
            }
        }
        else{
            e->ignore();
        }
    }
    else e->ignore();
    QCoreApplication::processEvents();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e){
    if(pieceChosen&&placeChosen){
        if(dfs[locateChessBoard.first][locateChessBoard.second]==1){
            QByteArray *array=new QByteArray;
            QString str="{\"f1\":%1,\"s1\":%2,\"f2\":%3,\"s2\":%4}";
            str=str.arg(locatePlayerChess.first).arg(locatePlayerChess.second).arg(locateChessBoard.first).arg(locateChessBoard.second);
            array->clear();
            array->append(str.toStdString());
            this->socket->write(array->data());
            QTest::qSleep(30);
            move();
            updateInfo();
            QCoreApplication::processEvents();
        }
    }
    else e->ignore();
}
