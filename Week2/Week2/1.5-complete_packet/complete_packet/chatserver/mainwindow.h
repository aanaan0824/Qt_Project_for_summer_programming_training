#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtNetwork/QTcpServer>
//#include <QtNetwork/QTcpSocket>
//#include <QtNetwork/QHostAddress>
#include <QtNetwork>
#include "netcom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initServer();

public slots:
    void acceptConnection();
    void recvMessage();
    void recvPacket(char *);

private slots:
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;
    NetCom packer;
};

#endif // MAINWINDOW_H
