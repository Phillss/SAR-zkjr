#include "conratio.h"
#include "ui_conratio.h"

ConRatio::ConRatio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConRatio)
{
    ui->setupUi(this);
    setWindowTitle("对比度调节");
}

ConRatio::~ConRatio()
{
    delete ui;
}
