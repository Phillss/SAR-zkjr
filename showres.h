#ifndef SHOWRES_H
#define SHOWRES_H

#include <QMainWindow>

namespace Ui {
class ShowRes;
}

class ShowRes : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShowRes(QWidget *parent = nullptr);
    ~ShowRes();

private:
    Ui::ShowRes *ui;
};

#endif // SHOWRES_H
