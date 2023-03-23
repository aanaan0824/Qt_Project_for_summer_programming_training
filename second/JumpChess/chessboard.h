#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPicture>
#include <QPaintEvent>
#include <QDebug>

namespace Ui {
class chessboard;
}

class chessboard : public QWidget
{
    Q_OBJECT

public:
    explicit chessboard(QWidget *parent = nullptr);
    ~chessboard();
    void paintcircle(int, int);
    void drawchess(int, int, int);
    struct pos{
        int chesscolor;
        int x,y;
    };

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::chessboard *ui;
    bool whether_after_click;
    int which_player_is_playing;
    pos thisboard[18][14];
    pos click_a_point(int x,int y);
};

#endif // CHESSBOARD_H
