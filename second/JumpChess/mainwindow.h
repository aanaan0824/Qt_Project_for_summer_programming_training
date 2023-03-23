#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QEvent>
#include <QPainter>
#include <QDebug>
#include <QString>
#include "chessboard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //void mousePressEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);
    class player;

private slots:


private:
    Ui::MainWindow *ui;
    chessboard bo;
};
#endif // MAINWINDOW_H
