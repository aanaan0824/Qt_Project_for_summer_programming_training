#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(const QString &initialText,
                              bool isBackward, QWidget *parent = 0);

    bool isBackward() const;
    QString searchText() const;

    ~SearchDialog();

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
