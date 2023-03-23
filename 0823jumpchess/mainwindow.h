#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QAction>
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QPicture>
#include <QPaintEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTime>
#include <QTimer>
#include <QRegularExpression>

#include "dialog_create_connection.h"
#include "dialog_connect_to_server.h"
#include "dialog_win_lose.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintcircle(int, int);
    void drawchess(int, int, int);
    struct pos{
        int chesscolor;
        int x,y;
        int row,queue;
    };
    struct player{
        int color;
        int stepnum;
        int overtime;
        bool win;
    };
    player this_player;
    player that_player;
    void win_pop_up();
    void lose_pop_up();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void on_actionCreate_the_connection_triggered();
    void on_actionConnect_to_Server_triggered();

    void on_actionStart_triggered();

    void on_actionAdmit_defeat_triggered();

public slots:
    void recvMessage();
    void acceptConnection();

private:
    Ui::MainWindow *ui;
    QTcpServer *listenSocket;
    QTcpSocket *readWriteSocket;
    Dialog_create_connection *create;
    Dialog_connect_to_server *connectto;
    bool whether_a_server;
    QString Serverip;
    QTimer *mytimer;
    int mytimerid;
    int timernum;
    Dialog_win_lose *result;

    //former variable
    bool whether_after_click;
    int which_color_is_playing;
    pos thisboard[18][14];
    pos click_a_point(int x,int y);
    pos the_chess_float;
    bool can_this_chess_be_there(pos ori,pos next);
    bool are_you_neighbors(pos a,pos b);
    pos the_next_pos(pos ori,pos next);
    int canit[18][14];
    void dfs(int, int, int, int);
    int chessnum;
    int roundnum;
    bool whether_start_the_game;
    bool can_start_the_game;
    bool first_start_timer;
    int whether_win();
    int eq(int x,int y);
};
#endif // MAINWINDOW_H
