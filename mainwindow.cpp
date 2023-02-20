#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QElapsedTimer>
#include <QScreen>
#include <QRect>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

#define MIN_height 740
#define MIN_width 580
#define MAX_height 1600
#define MAX_width 1800
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QScreen*> list_Screen =QGuiApplication::screens();
    QRect rect=list_Screen.at(0)->geometry();
    int desk_width=rect.width();
    int desk_height=rect.height();
    int pos_x=desk_width/6,pos_y=desk_height/5,adjust_wid=desk_width*2/3,adjust_hei=desk_height*2/3;
    setGeometry(pos_x,pos_y,adjust_wid,adjust_hei);
    setWindowTitle("SAR图像地面站辅助判读系统");
    setMinimumSize(MIN_height,MIN_width);
    setMaximumSize(MAX_height,MAX_width);
    homePath=QDir::homePath();
    loadpath=QCoreApplication::applicationDirPath();
    loadfilename=loadpath+"/LDfile";
    QDir remove(loadfilename);
    if(remove.exists()){
        remove.removeRecursively();
    }
//    QElapsedTimer t;
//    t.start();
//    while(t.elapsed()<2500)
//        QCoreApplication::processEvents();
    model=new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(model);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("文件列表"));
//    for(int i=0;i<20;i++){
//        model->setItem(i,0,new QStandardItem("onstart.jpg"));
//        model->item(i,0)->setChild(0,0,new QStandardItem("23"));
//        model->item(i,0)->setChild(1,0,new QStandardItem("23"));
//    }
    connect(ui->treeView,&QTreeView::clicked,this,&MainWindow::select);
}
void MainWindow::select(const QModelIndex &index){
    QVariant v=index.data();
    QString n=v.toString();
    QString temp=loadfilename+"/"+n;
    QGraphicsScene *scen=new QGraphicsScene;
    scen->addPixmap(QPixmap(temp));
    ui->graphicsView->setScene(scen);
    ui->graphicsView->show();
//    image->load()
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actiondaoru_triggered()
{
    QString currFilePath=QFileDialog::getOpenFileName(this,homePath);
    QFileInfo fileinfor(currFilePath);
    QString filefull=fileinfor.fileName();
    QDir loadFile(loadfilename);
    if(!loadFile.exists()){
        loadFile.mkdir(loadfilename);
    }
    if(!QFile::exists(loadfilename+"/"+filefull)){
        QFile::copy(currFilePath,loadfilename+"/"+filefull);
    }
    QStandardItem *item=new QStandardItem(fileinfor.baseName());
    qDebug()<<fileinfor.baseName();
    int currrowcount=model->rowCount();
    model->setItem(currrowcount,0,item);
}

