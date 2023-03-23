#ifndef DIALOG_CONNECT_TO_SERVER_H
#define DIALOG_CONNECT_TO_SERVER_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog_connect_to_server;
}

class Dialog_connect_to_server : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_connect_to_server(QWidget *parent = nullptr);
    ~Dialog_connect_to_server();
    QString getServerIP();

private:
    Ui::Dialog_connect_to_server *ui;
};

#endif // DIALOG_CONNECT_TO_SERVER_H
