#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect(ui->treeView,&QTreeView::clicked,this,&MainWindow::select);
    scene=new QGraphicsScene();
    view=new QGraphicsView(scene);
    view->setParent(ui->center_widget);
}
void MainWindow::select(const QModelIndex &index){
    scene->clear();
    QVariant v=index.data();
    QString n=v.toString();
    QString temp=loadfilename+"/"+n;
    image_scaled_widget *ll=new image_scaled_widget();
    QPixmap *pix=new QPixmap();
    pix->load(temp);
    ll->change_new_image(pix,NUL);
    scene->addItem(ll);
    view->fitInView(ll,Qt::KeepAspectRatio);
    view->show();
    onclicked=temp;
    currentPix=pix;
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
    int currrowcount=model->rowCount();
    model->setItem(currrowcount,0,item);
}


void MainWindow::on_actiondaochu_triggered()
{
    ef=new ExportFile(this);
    ef->show();
}


void MainWindow::on_action_triggered()
{
    res=new ShowRes(this);
    res->show();
}

//图像旋转
void MainWindow::on_actiontuxiang_triggered()
{
    scene->clear();
    bool a=t!=NULL;
    bool b=t==onclicked;
    qDebug()<<a<<" ::"<<b<<t<<":"<<onclicked;
    if(t!=NULL&&t==onclicked){
        re=(re+90)%360;
        qDebug()<<re;
    }else{
        re=90;
    }
    QMatrix matrix;
    matrix.rotate(re);
    if(currentPix!=NULL){
        QPixmap tt=currentPix->transformed(matrix,Qt::SmoothTransformation);
        image_scaled_widget *ls=new image_scaled_widget();
        ls->change_new_image(&tt,NUL);
        scene->addItem(ls);
        view->fitInView(ls,Qt::KeepAspectRatio);
        view->show();
    }
    t=onclicked;
}

