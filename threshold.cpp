#include "threshold.h"
#include "ui_threshold.h"
#include <QDebug>

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
    processPath=QCoreApplication::applicationDirPath();
    outputdir=processPath+"/LDfile/";
}
void threshold::setMaxt(int t){
    this->max_t=t;
    ui->horizontalSlider->setMaximum(max_t);
}
void threshold::setMint(int t){
    this->min_t=t;
    ui->horizontalSlider->setMinimum(min_t);
}
void threshold::setDefault(int t){
    this->default_t=t;
    ui->horizontalSlider->setValue(default_t);
}
void threshold::setMaxH(int h){
    this->max_height=h;
}
void threshold::setMaxW(int h){
    this->max_width=h;
}
void threshold::setFalg(bool flag){
    this->flag=flag;
    ui->widget_2->setVisible(flag);
}
void threshold::setFilename(QString name){
    this->filename=name;
}
void threshold::setalgorithmpath(QString name){
    this->algorithmpath=name;
}
void threshold::setalgorithmDirName(QString name){
    this->algorithmDirName=name;
}
void threshold::setAlgorithmName(QString name){
    this->algorithmName=name;
}
void threshold::setDistname(QString name){
    this->distname=name;
}
void threshold::setAlgorithmCn(QString name){
    this->algorithmnameCn=name;
}
threshold::~threshold()
{
    delete ui;
}

void threshold::on_buttonBox_accepted()
{
    QProcess *p1=new QProcess(this);
    QString arg1=algorithmpath+"/"+algorithmName;//算法脚本文件
    QString arg2=filename;//图片文件的全路径
    QString arg3=QFileInfo(filename).baseName();//图片文件的裸称
    QString arg4=profileDir+"/";//配置文件所在文件夹
    QString arg5=outputdir+algorithmnameCn+distname+"-";//文件输出全路径
//    QString cmd="python3 "+algorithmpath+"/"+algorithmName+" "+filename+" "+QFileInfo(filename).baseName()+" "+profileDir+"/ "+outputdir+algorithmName+distname;
//    qDebug()<<arg1;
//    qDebug()<<arg2;
//    qDebug()<<arg3;
//    qDebug()<<arg4;
//    qDebug()<<arg5;
    QString cmd="python3 "+arg1+" "+arg2+" "+arg3+" "+arg4+" "+arg5;
    qDebug()<<cmd;
    if(!profileDir.isEmpty()){
        p1->start(cmd);
//        qDebug()<<QString::fromLocal8Bit(p1->readAllStandardOutput().data());
    }
}

//加载配置文件
void threshold::on_pushButton_clicked()
{
    QString profile=QFileDialog::getExistingDirectory(this);
    ui->lineEdit->setText(profile);
    if(!profile.isEmpty()){
        profileDir=profile;
    }
}

