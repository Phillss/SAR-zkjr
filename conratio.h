#ifndef CONRATIO_H
#define CONRATIO_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class ConRatio;
}

class ConRatio : public QDialog
{
    Q_OBJECT

public:
    explicit ConRatio(QWidget *parent = nullptr);
    ~ConRatio();
signals:
    void withParaSignal(int value);
private slots:
    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_sliderMoved(int position);

    void on_spinBox_valueChanged(int arg1);
    void sendValueToBase(int vlaue);

private:
    Ui::ConRatio *ui;

};

#endif // CONRATIO_H
