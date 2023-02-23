#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <QDialog>

namespace Ui {
class threshold;
}

class threshold : public QDialog
{
    Q_OBJECT

public:
    explicit threshold(QWidget *parent = nullptr);
    ~threshold();

private:
    Ui::threshold *ui;
};

#endif // THRESHOLD_H
