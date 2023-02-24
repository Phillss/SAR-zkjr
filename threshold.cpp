#include "threshold.h"
#include "ui_threshold.h"

threshold::threshold(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::threshold)
{
    ui->setupUi(this);
    setWindowTitle("算法参数调节");
    connect(ui->horizontalSlider,&QSlider::valueChanged,ui->spinBox,&QSpinBox::setValue);
    //函数QSpinBox::valueChanged出现了函数重载（int或char），需要定义一个函数指针排除二义性
    void (QSpinBox:: *box_single)(int)=&QSpinBox::valueChanged;
    connect(ui->spinBox,box_single,ui->horizontalSlider,&QSlider::setValue);
    ui->height->setValidator(new QIntValidator(0,max_height,this));
    ui->width->setValidator(new QIntValidator(0,max_width,this));
    ui->horizontalSlider->setValue(default_t);
    ui->widget_2->setVisible(flag);
}

threshold::~threshold()
{
    delete ui;
}
