#ifndef DIALOG_CREATE_CONNECTION_H
#define DIALOG_CREATE_CONNECTION_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog_create_connection;
}

class Dialog_create_connection : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_create_connection(QWidget *parent = nullptr);
    ~Dialog_create_connection();
    void set_the_label(QString in);

private slots:
    //void on_buttonBox_rejected();

private:
    Ui::Dialog_create_connection *ui;

};

#endif // DIALOG_CREATE_CONNECTION_H
