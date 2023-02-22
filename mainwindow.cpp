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

    QElapsedTimer t;
    t.start();
    while(t.elapsed()<2500)
        QCoreApplication::processEvents();
    model=new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(model);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("文件列表"));
    connect(ui->treeView,&QTreeView::clicked,this,&MainWindow::select);
    scene=new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    view=ui->graphicsView;
    view->setParent(ui->center_widget);
    alertDia=new Alert();
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
//    QString currFilePath=QFileDialog::getOpenFileName(this,homePath);
    QStringList file_list,output_name,str_path_list;
    QString str_path;
    str_path_list=QFileDialog::getOpenFileNames(this);
    for(int i=0;i<str_path_list.size();i++){
        str_path=str_path_list[i];
        QFileInfo fileinfor=QFileInfo(str_path);
        QString filefull=fileinfor.fileName();
        QDir loadFile(loadfilename);
        if(!loadFile.exists()){
            loadFile.mkdir(loadfilename);
        }
        if(!QFile::exists(loadfilename+"/"+filefull)){
            QFile::copy(str_path,loadfilename+"/"+filefull);
        }
        QStandardItem *item=new QStandardItem(fileinfor.baseName());
        int currrowcount=model->rowCount();
        model->setItem(currrowcount,0,item);
    }
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
    if(t!=NULL&&t==onclicked){
        re=(re+90)%360;
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
    }else{

    }
    t=onclicked;
}

//水平翻转
void MainWindow::on_actiontuxiang_2_triggered()
{
    scene->clear();
    if(onclicked!=NULL){
        if(t==onclicked){
            mirrored=pre.mirrored(true,false);
        }else{
            QImage im(onclicked);
            mirrored=im.mirrored(true,false);
        }
        pre=mirrored;
        QPixmap mp=QPixmap::fromImage(mirrored);
        image_scaled_widget *ls=new image_scaled_widget();
        ls->change_new_image(&mp,NUL);
        scene->addItem(ls);
        view->fitInView(ls,Qt::KeepAspectRatio);
        view->show();

    }
    t=onclicked;
}

//垂直翻转
void MainWindow::on_action_2_triggered()
{
    scene->clear();
    if(onclicked!=NULL){
        if(t==onclicked){
            mirrored=pre.mirrored(true,true);
        }else{
            QImage im(onclicked);
            mirrored=im.mirrored(true,true);
        }
        pre=mirrored;
        QPixmap mp=QPixmap::fromImage(mirrored);
        image_scaled_widget *ls=new image_scaled_widget();
        ls->change_new_image(&mp,NUL);
        scene->addItem(ls);
        view->fitInView(ls,Qt::KeepAspectRatio);
        view->show();
    }
    t=onclicked;
}


void MainWindow::on_actionduibi_triggered()
{
    ratio=new ConRatio(this);
    ratio->show();
}

