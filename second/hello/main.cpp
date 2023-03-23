#include "mainwindow.h"
#include "form123.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Form123 f;
    f.show();
    return a.exec();
}
