#include "conratio.h"
#include "ui_conratio.h"

ConRatio::ConRatio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConRatio)
{
    ui->setupUi(this);
    setWindowTitle("对比度调节");
    connect(ui->horizontalSlider,&QSlider::valueChanged,ui->spinBox,&QSpinBox::setValue);
    //函数QSpinBox::valueChanged出现了函数重载（int或char），需要定义一个函数指针排除二义性
    void (QSpinBox:: *box_single)(int)=&QSpinBox::valueChanged;
    connect(ui->spinBox,box_single,ui->horizontalSlider,&QSlider::setValue);

    connect(ui->horizontalSlider,&QSlider::valueChanged,this,&ConRatio::sendValueToBase);
    connect(ui->spinBox,box_single,this,&ConRatio::sendValueToBase);
}

ConRatio::~ConRatio()
{
    delete ui;
}
//子窗口发送调节值到父窗口
void ConRatio::sendValueToBase(int value){
    emit withParaSignal(value);
}
void ConRatio::on_horizontalSlider_actionTriggered(int action)
{
}

//滑动条调节
void ConRatio::on_horizontalSlider_sliderMoved(int position)
{

}

//旋转框调节
void ConRatio::on_spinBox_valueChanged(int arg1)
{
}


