#include "exportfile.h"
#include "ui_exportfile.h"

ExportFile::ExportFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportFile)
{
    ui->setupUi(this);
    setWindowTitle("文件导出");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("导出");
    exmodel=new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(exmodel);
    exmodel->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("文件列表"));
    output=QCoreApplication::applicationDirPath()+"/LDfile/";
}
void ExportFile::recieveFromMain(QStandardItemModel *model){
    if(model->rowCount()==0){
        alert=new Alert();
        alert->setParent(this);
        alert->setMessage("无项目导出!");
        alert->show();
    }else{
        for(int curIndex=0;curIndex<model->rowCount();curIndex++){
            QModelIndex si=model->index(curIndex,0);
            QStandardItem *curItem=model->itemFromIndex(si);
            QString curItemName=curItem->text();
            QStandardItem *items=new QStandardItem(curItemName);
            items->setCheckable(true);
            exmodel->setItem(curIndex,0,items);
        }
    }
}
void ExportFile::exportFiles(QStandardItemModel *model){
}
ExportFile::~ExportFile()
{
    delete ui;
}

void ExportFile::on_buttonBox_accepted()
{
    exFiles.clear();
    for(int curIndex=0;curIndex<exmodel->rowCount();curIndex++){
        QStandardItem *checkitem=exmodel->item(curIndex,0);
        if(checkitem->checkState()==Qt::Checked){
            exFiles.append(checkitem->text());
        }
    }
    if(exFiles.count()==0){
        alert=new Alert();
        alert->setMessage("无选中导出文件！");
        alert->show();
    }else{
        QString distpath=QFileDialog::getExistingDirectory();
        qDebug()<<distpath;
        for(int i=0;i<exFiles.count();i++){
            qDebug()<<exFiles[i];
        }
    }
}

