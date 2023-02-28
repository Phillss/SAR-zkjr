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

    setMinimumSize(MIN_height,MIN_width);
    setMaximumSize(MAX_height,MAX_width);
    homePath=QDir::homePath();
    loadpath=QCoreApplication::applicationDirPath();
//    output=loadpath+"/LDfile/";
    loadfilename=loadpath+"/LDfile";
    QDir remove(loadfilename);
    if(remove.exists()){
        remove.removeRecursively();
    }
    loadXML(loadpath+"/ini.xml");//ini.xml
    setWindowTitle(this->mainWindowHeader);
    algorithmpath=loadpath+"/"+algorithmpath;

    QDir loadAlgorithm(algorithmpath);
    if(!loadAlgorithm.exists()){
        loadAlgorithm.mkdir(algorithmpath);
    }
//ini window
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<500)
        QCoreApplication::processEvents();
    model=new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(model);
    model->setHorizontalHeaderLabels(QStringList()<<filelist);
    connect(ui->treeView,&QTreeView::clicked,this,&MainWindow::select);
    scene=new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    view=ui->graphicsView;
    view->setParent(ui->center_widget);

    alertDia=new Alert();
    ef=new ExportFile(this);
    connect(this,&MainWindow::sendToExport,ef,&ExportFile::recieveFromMain);
    ratio=new ConRatio(this);
    connect(ratio,&ConRatio::withParaSignal,this,&MainWindow::recieveValue);//接收子窗口的调节数值

    nullmessage="请导入图片或选择操作对象!";
    //文件折叠效果设置
}
//文件选取
void MainWindow::select(const QModelIndex &index){
    scene->clear();
    QModelIndex pindex=index.parent();
    if(pindex.isValid()){
        qDebug()<<"子结点";
        QVariant childv=index.data();
        QString childname=childv.toString();
        QString childtemp=loadfilename+"/"+childname;
        image_scaled_widget *cl=new image_scaled_widget();
        QPixmap cpix(childtemp);
        currImg=QImage(childtemp);
        re=0;
        cl->change_new_image(&cpix,NUL);
        scene->addItem(cl);
        view->fitInView(cl,Qt::KeepAspectRatio);
        view->show();
        onclicked=childtemp;
        onclickedRow=index.row();
    }else{
        QVariant v=index.data();
        QString n=v.toString();
        QString temp=loadfilename+"/"+n;
        image_scaled_widget *ll=new image_scaled_widget();
        QPixmap pix(temp);
        currImg=QImage(temp);
        re=0;
        ll->change_new_image(&pix,NUL);
        scene->addItem(ll);
        view->fitInView(ll,Qt::KeepAspectRatio);
        view->show();
        onclicked=temp;
        onclickedRow=index.row();
    }
}
//算法选取
void MainWindow::algori_select(const QModelIndex &index){
    holder=new threshold();
    connect(holder,&threshold::returnResult,this,&MainWindow::appendAlgorithmResult);
    int mapindex=nameToindex.value(index.data().toString());
    Algom *al=hash.value(mapindex);
    if(al->getNamepy()==""||!QFile(algorithmpath+"/"+al->getNamepy()).exists()){
        alertDia->setMessage("算法文件加载失败！");
        alertDia->show();
    }else{
        holder->setDefault(al->getDefaultt());
        holder->setMint(al->getmint());
        holder->setMaxt(al->getmaxt());
        holder->setFalg(al->getFlag());
        holder->setProfile(al->getProfileFlag());
        holder->setFilename(onclicked);
        holder->setAlgorithmName(al->getNamepy());
        holder->setAlgorithmCn(al->getNamecn());
        holder->setalgorithmpath(algorithmpath);
        if(onclickedRow==-1||onclickedRow>model->rowCount()){
            alertDia->setMessage(nullmessage);
            alertDia->show();
        }else{
            holder->setDistname(QString::number(model->item(onclickedRow,0)->rowCount()+1));
            holder->show();
        }
    }
}
//接收参数调节结果
void MainWindow::appendAlgorithmResult(QString distname){
    if(onclickedRow==-1){
        alertDia->setMessage(nullmessage);
        alertDia->show();
    }else{
        int childRows=model->item(onclickedRow,0)->rowCount();
        model->item(onclickedRow,0)->setChild(childRows,0,new QStandardItem(distname));
//        model->item(onclickedRow,0)->child(childRows,0)--;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//文件导入窗口
void MainWindow::on_actiondaoru_triggered()
{
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
        QStandardItem *item=new QStandardItem(filefull);
        int currrowcount=model->rowCount();
        model->setItem(currrowcount,0,item);
//        model->item(currrowcount,0)->setChild(0,0,new QStandardItem("算法处理结果1"));
    }
}

//文件导出窗口
void MainWindow::on_actiondaochu_triggered()
{
    emit sendToExport(model);
    ef->show();
}

//对比度调节窗口
void MainWindow::on_actionduibi_triggered()
{
//    ratio=new ConRatio(this);
//    connect(ratio,&ConRatio::withParaSignal,this,&MainWindow::recieveValue);//接收子窗口的调节数值
    if(onclickedRow==-1){
        alertDia->setMessage(nullmessage);
        alertDia->show();
    }else{
        ratio->show();
    }
}
//对比度调节
void MainWindow::recieveValue(int value){
    if(onclickedRow==-1){
        alertDia->setMessage(nullmessage);
        alertDia->show();
    }else{
        if(onclicked!=NULL){
            scene->clear();
            if(t==onclicked&&!currImg.isNull()){
                currImg=AdjustContrast(currImg,value);
            }else{
                QImage img(onclicked);
                currImg=AdjustContrast(img,value);
            }
//            pre=mirrored;
            QPixmap mp=QPixmap::fromImage(currImg);
            image_scaled_widget *ls=new image_scaled_widget();
            ls->change_new_image(&mp,NUL);
            scene->addItem(ls);
            view->fitInView(ls,Qt::KeepAspectRatio);
            view->show();
        }
        t=onclicked;
    }
}

//图像旋转
void MainWindow::on_actiontuxiang_triggered()
{
    if(onclickedRow==-1){
        alertDia->setMessage(nullmessage);
        alertDia->show();
    }else{
        scene->clear();
        if(t!=NULL&&t==onclicked){
            re=90;
        }else{
            re=90;
            QImage im(onclicked);
            currImg=im;
        }
        QMatrix matrix;
        matrix.rotate(re);
        if(!currImg.isNull()){
            currentPix=QPixmap::fromImage(currImg);
            QPixmap tt=currentPix.transformed(matrix,Qt::SmoothTransformation);
            image_scaled_widget *ls=new image_scaled_widget();
            ls->change_new_image(&tt,NUL);
            scene->addItem(ls);
            view->fitInView(ls,Qt::KeepAspectRatio);
            view->show();
            currImg=tt.toImage();
        }
        t=onclicked;
    }
}

//水平翻转
void MainWindow::on_actiontuxiang_2_triggered()
{
    if(onclickedRow==-1){
        alertDia->setMessage(nullmessage);
        alertDia->show();
    }else{
        scene->clear();
        if(onclicked!=NULL){
            if(t==onclicked&&!currImg.isNull()){
                currImg=currImg.mirrored(true,false);
            }else{
                QImage im(onclicked);
                currImg=im.mirrored(true,false);
            }
            QPixmap mp=QPixmap::fromImage(currImg);
            image_scaled_widget *ls=new image_scaled_widget();
            ls->change_new_image(&mp,NUL);
            scene->addItem(ls);
            view->fitInView(ls,Qt::KeepAspectRatio);
            view->show();
        }
        t=onclicked;
    }
}

//垂直翻转
void MainWindow::on_action_2_triggered()
{
    if(onclickedRow==-1){
        alertDia->setMessage(nullmessage);
        alertDia->show();
    }else{
        scene->clear();
        if(onclicked!=NULL){
            if(t==onclicked&&!currImg.isNull()){
                currImg=currImg.mirrored(true,true);
            }else{
                QImage im(onclicked);
                currImg=im.mirrored(true,true);
            }
            QPixmap mp=QPixmap::fromImage(currImg);
            image_scaled_widget *ls=new image_scaled_widget();
            ls->change_new_image(&mp,NUL);
            scene->addItem(ls);
            view->fitInView(ls,Qt::KeepAspectRatio);
            view->show();
        }
        t=onclicked;
    }
}
//显示处理结果窗口
void MainWindow::on_action_triggered()
{
    res=new ShowRes(this);
    res->show();
}
//加载配置文件
void MainWindow::loadXML(QString xmlpath){
    algorithmmodel=new QStandardItemModel(ui->treeView_2);
    ui->treeView_2->setModel(algorithmmodel);//设置数据模型
    connect(ui->treeView_2,&QTreeView::clicked,this,&MainWindow::algori_select);
    QDomDocument doc;
    QFile file(xmlpath);
    if (xmlpath.isEmpty()||!file.open(QIODevice::ReadOnly)){
        alertDia->setMessage("初始化失败！");
        return;
    }
    if (!doc.setContent(&file))
    {
        alertDia->setMessage("初始化失败！");
        file.close();
        return;
    }
    file.close();
    if(doc.setContent(&file)){
        docelement=doc.documentElement();
        listDom(docelement);
    }
    algorithmHeader.append(algorithmlistname);
    algorithmmodel->setHorizontalHeaderLabels(algorithmHeader);
}
//解析配置文件
void MainWindow::listDom(QDomElement& docElem)
{
    QDomNode node = docElem.firstChild();
    QString tagname;
    if(node.toElement().isNull()){
        return;
    }
    while(!node.isNull()){
        tagname=node.toElement().tagName();
        if(tagname=="onstartpic"){

        }else if(tagname=="onstartheader"){

        }else if(tagname=="mainwindowheader"){
            this->mainWindowHeader=node.toElement().text();
        }else if(tagname=="algorithmpath"){
            this->algorithmpath=node.toElement().text();
        }else if(tagname=="algorithmlist"){
            listAlgorihm(node);
        }else if(tagname=="filelistname"){
            this->filelist=node.toElement().text();
        }else if(tagname=="algorithmlistname"){
            this->algorithmlistname=node.toElement().text();
        }
        node=node.nextSibling();
    }
}
//解析配置文件中算法列表
void MainWindow::listAlgorihm(QDomNode& algorithmNodes){
    QDomNode algorithmsNode,propertyNode,thresholdNode,window;
    QString tagname,thresholdname;
    algorithmsNode=algorithmNodes.toElement().firstChild();//algorithm
    int index=0;
    while(!algorithmsNode.isNull()){//遍历每个算法
        QString name=algorithmsNode.toElement().firstChild().toElement().text();
//        qDebug()<<name;
        propertyNode=algorithmsNode.toElement().firstChild();
//        qDebug()<<propertyNode.toElement().text();
        Algom *algo=new Algom();
        while(!propertyNode.isNull()){//解析每个算法的属性
            tagname=propertyNode.toElement().tagName();
//            qDebug()<<tagname;
            if(tagname=="algorithm-name"){
                algo->setNamecn(propertyNode.toElement().text());
//                qDebug()<<propertyNode.toElement().text();
            }else if(tagname=="algorithm-local"){
                algo->setNamepy(propertyNode.toElement().text());
            }else if(tagname=="algorithm-threshold"){
                thresholdNode=propertyNode.toElement().firstChild();
                while(!thresholdNode.isNull()){
                    thresholdname=thresholdNode.toElement().tagName();
                    if(thresholdname=="default"){
                        algo->setDefaultt(thresholdNode.toElement().text().toInt());
                    }else if(thresholdname=="min"){
                        algo->setmint(thresholdNode.toElement().text().toInt());
                    }else if(thresholdname=="max"){
                        algo->setmaxt(thresholdNode.toElement().text().toInt());
                    }
                    thresholdNode=thresholdNode.nextSibling();
                }
            }else if(tagname=="window_size"){
                algo->setFlag(true);
                window=propertyNode.toElement().firstChild();
                while(!window.isNull()){
                    if(window.toElement().tagName()=="height"){
                        if(!window.toElement().text().isEmpty())
                            algo->setWinwid(window.toElement().text().toInt());
                    }else if(window.toElement().tagName()=="width"){
                        if(!window.toElement().text().isEmpty())
                            algo->setWinhei(window.toElement().text().toInt());
                    }
                    window=window.nextSibling();
                }
            }else if(tagname=="profile"){
                algo->setProfileFlag(true);
            }
            propertyNode=propertyNode.nextSibling();
        }
        if(!nameToindex.contains(algo->getNamecn())){
            hash.insert(index,algo);
            nameToindex.insert(algo->getNamecn(),index);
            index++;
            QStandardItem *algori_item=new QStandardItem(name);
            int algori_count=algorithmmodel->rowCount();
            algorithmmodel->setItem(algori_count,0,algori_item);
            algorithmsNode=algorithmsNode.nextSibling();
        }
    }
}
//搜索算法
void MainWindow::on_pushButton_clicked()
{
    QString algoname=ui->lineEdit->text();
    if(!algoname.isEmpty()){
        QStandardItemModel *searchModel=new QStandardItemModel(ui->treeView_2);
        ui->treeView_2->setModel(searchModel);//设置数据模型
        searchModel->setHorizontalHeaderLabels(QStringList()<<"搜索结果");
        if(nameToindex.contains(algoname)){//full match
            QStandardItem *search=new QStandardItem(algoname);
            int search_count=searchModel->rowCount();
            searchModel->setItem(search_count,0,search);
        }else{
            alertDia->setMessage("搜索结果不存在!");
            alertDia->show();
            ui->lineEdit->setText("");
            ui->treeView_2->setModel(algorithmmodel);//设置数据模型
        }
    }
}


void MainWindow::on_lineEdit_editingFinished()
{

}

//显示全部算法
void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    if(arg1.isEmpty()){
        ui->treeView_2->setModel(algorithmmodel);//设置数据模型
    }
}

