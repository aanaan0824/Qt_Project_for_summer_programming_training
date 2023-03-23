#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSignalMapper>

#define MAX_CONNECTIONS 1024

namespace Ui {
class MainWindow;
}

class QTcpSocket;
class QTcpServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void server_New_Connect();
    void socket_Read_Data(int sock_idx);
private slots:
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket* m_working_socks[MAX_CONNECTIONS];
    bool m_used[MAX_CONNECTIONS];
    int m_active_conn;
    int m_pivot;
    QSignalMapper* m_sigmap;
};

#endif // MAINWINDOW_H
