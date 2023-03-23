#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWindow; }
QT_END_NAMESPACE

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);
    ~MyWindow();
    void myFunction();

private slots:
    void on_searchButton_clicked();

private:
    Ui::MyWindow *ui;
    QSettings settings;
};
#endif // MYWINDOW_H
