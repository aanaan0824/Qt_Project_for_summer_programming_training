#ifndef DIALOG_WIN_LOSE_H
#define DIALOG_WIN_LOSE_H

#include <QDialog>

namespace Ui {
class Dialog_win_lose;
}

class Dialog_win_lose : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_win_lose(QWidget *parent = nullptr);
    ~Dialog_win_lose();
    void set_the_label(QString in);

private:
    Ui::Dialog_win_lose *ui;
};

#endif // DIALOG_WIN_LOSE_H
