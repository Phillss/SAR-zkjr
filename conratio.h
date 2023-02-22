#ifndef CONRATIO_H
#define CONRATIO_H

#include <QDialog>

namespace Ui {
class ConRatio;
}

class ConRatio : public QDialog
{
    Q_OBJECT

public:
    explicit ConRatio(QWidget *parent = nullptr);
    ~ConRatio();

private:
    Ui::ConRatio *ui;
};

#endif // CONRATIO_H
