#include "showres.h"
#include "ui_showres.h"

ShowRes::ShowRes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowRes)
{
    ui->setupUi(this);
    setWindowTitle("处理结果");
}

ShowRes::~ShowRes()
{
    delete ui;
}
