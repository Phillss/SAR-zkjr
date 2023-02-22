#include "alert.h"
#include "ui_alert.h"

Alert::Alert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Alert)
{
    ui->setupUi(this);
    setWindowTitle("提示");
    ui->alertMessage->toPlainText();
}

Alert::~Alert()
{
    delete ui;
}
