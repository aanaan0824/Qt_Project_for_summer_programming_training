#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include "chess.h"
#include "dialog1.h"
#include "dialog2.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int benchmark_x=width()*2/3;
    int benchmark_y=50;

    bool pieceChosen=false;
    bool placeChosen=false;
    std::pair<int,int>locateChessBoard={-1,-1};
    std::pair<int,int>locatePlayerChess={-1,-1};

    Chess *_chess[17][14];
    int dfs[17][14]={};
    Ui::MainWindow *ui;

    //网络连接
    QTcpServer *server=nullptr;
    QTcpSocket *socket=nullptr;
    Dialog1 *dlg1;
    Dialog2 *dlg2;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintChess(QPaintEvent *ev,Chess *c);
    void paintEvent(QPaintEvent *ev);

    //玩家信息
    bool start=false;
    bool isServer=false;
    int currentPlayerType=1;
    int myType=1;
    int round=0;
    int flag=0;

    //判断胜负
    void judgement();
    void winOrLose();
    int own=10;
    int opposite=0;
    int outOfTime=0;
    int win=-1;
    int lose=-1;
    void updateInfo();
    QTimer* timer;
    int timecount=20;

    //网络
    void initServer();
    void connectHost(QString qst);
    void assignMsg(int f1,int f2,int f3,int f4);

    //棋子移动
    Chess* leftUp(Chess* c);
    Chess* rightUp(Chess*);
    Chess* left(Chess*);
    Chess* right(Chess*);
    Chess* leftDown(Chess*);
    Chess* rightDown(Chess*);
    Chess* linkTo(int x,int y);
    void chessWalk();
    void chessJumpBfs(Chess *c, Chess *target);
    void preMove(Chess* c);
    void move();
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

public slots:
    void acceptConnection();
    void receiveMessage();
};
#endif // MAINWINDOW_H
