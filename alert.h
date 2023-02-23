#ifndef ALERT_H
#define ALERT_H

#include <QDialog>

namespace Ui {
class Alert;
}

class Alert : public QDialog
{
    Q_OBJECT

public:
    explicit Alert(QWidget *parent = nullptr);
    ~Alert();
    void setMessage(QString s);

private:
    Ui::Alert *ui;
};

#endif // ALERT_H
