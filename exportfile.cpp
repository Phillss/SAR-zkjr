#include "exportfile.h"
#include "ui_exportfile.h"

ExportFile::ExportFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportFile)
{
    ui->setupUi(this);
    setWindowTitle("文件导出");
}
void ExportFile::recieveFromMain(QStandardItemModel *model){
//    qDebug()<<"receive";
    exmodel=new QStandardItemModel(ui->treeView);
    QModelIndex in=model->index(0,0);
    if(in.isValid()){
        for(int curIndex=0;curIndex<model->rowCount(in);++curIndex){
            qDebug()<<"dsfs";
            QModelIndex si=model->index(curIndex,0,in);
            QStandardItem *curItem=model->itemFromIndex(si);
            if(curIndex){
                QString curItemName=curItem->text();
                qDebug()<<curItemName;
            }
        }
    }
}
ExportFile::~ExportFile()
{
    delete ui;
}

void ExportFile::on_buttonBox_accepted()
{
    qDebug()<<"df";
}

