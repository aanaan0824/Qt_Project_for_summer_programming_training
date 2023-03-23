#include "mywindow.h"
#include "ui_mywindow.h"
#include "searchdialog.h"

MyWindow::MyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyWindow)
{
    ui->setupUi(this);
}

void MyWindow::myFunction()
{
    SearchDialog dlg(settings.value("searchText","").toString(),
                     settings.value("searchBackward", false).toBool(), this);

    if(dlg.exec() == QDialog::Accepted)
    {
        bool backwards = dlg.isBackward();
        QString text = dlg.searchText();
        ui->searchLabel->setText(text);
        if (backwards)
            ui->dirLabel->setText("backward");
        else
            ui->dirLabel->setText("forward");
    }
}

MyWindow::~MyWindow()
{
    delete ui;
}


void MyWindow::on_searchButton_clicked()
{
    myFunction();
}

