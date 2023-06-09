#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = nullptr);
    ~Dialog2();
    bool isValidInput(QString s);
    bool received=0;
    QString q="";

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog2 *ui;
};

#endif // DIALOG2_H
