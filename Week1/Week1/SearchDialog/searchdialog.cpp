#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(const QString &initialText,
                           bool isBackward, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    ui->searchText->setText(initialText);
    if(isBackward)
        ui->directionBackward->setChecked(true);
    else
        ui->directionForward->setChecked(true);
}

bool SearchDialog::isBackward() const
{
    return ui->directionBackward->isChecked();
}

QString SearchDialog::searchText() const
{
    return ui->searchText->text();
}


SearchDialog::~SearchDialog()
{
    delete ui;
}
