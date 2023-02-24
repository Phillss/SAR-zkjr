#include "alert.h"
#include "ui_alert.h"

Alert::Alert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Alert)
{
    ui->setupUi(this);
    setWindowTitle("提示");

}
void Alert::setMessage(QString message){
    ui->label->setText(message);
}

Alert::~Alert()
{
    delete ui;
}
