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
    void setMaxt(int t);
    void setMint(int t);
    void setDefault(int t);
    void setMaxH(int h);
    void setMaxW(int w);
    void setFalg(bool flag);

private:
    Ui::threshold *ui;
    int max_t,min_t,default_t=5,max_height=10000,max_width=10000;
    bool flag=true;
};

#endif // THRESHOLD_H
