#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPicture>
#include <QPaintEvent>
#include <QDebug>
#include <QTimer>

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

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    //void timerEvent(QTimerEvent *event);

private slots:
    void on_actionCreate_Connection_triggered();

private:
    Ui::MainWindow *ui;
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
};
#endif // MAINWINDOW_H
