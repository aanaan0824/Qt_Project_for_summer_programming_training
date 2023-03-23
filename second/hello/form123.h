#ifndef FORM123_H
#define FORM123_H

#include <QWidget>
#include <QPainter>
#include <QImage>

namespace Ui {
class Form123;
}

class Form123 : public QWidget
{
    Q_OBJECT

public:
    explicit Form123(QWidget *parent = nullptr);
    ~Form123();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Form123 *ui;
};

#endif // FORM123_H
