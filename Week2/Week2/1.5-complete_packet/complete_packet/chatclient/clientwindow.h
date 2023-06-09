#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "netcom.h"

namespace Ui {
class clientWindow;
}

class clientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientWindow(QWidget *parent = 0);
    ~clientWindow();

    void connectHost();

public slots:
    void recvMessage();
    void recvPacket(char *);


private slots:
    void on_sendButton_clicked();

private:
    Ui::clientWindow *ui;
    QTcpSocket *readWriteSocket;
    NetCom packer;
};

#endif // CLIENTWINDOW_H
