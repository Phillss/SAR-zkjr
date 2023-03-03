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
    input=QCoreApplication::applicationDirPath()+"/LDfile/";
    QStringList zipfile;
    zipfile<<"gz"<<"bz2";
    ui->comboBox->addItems(zipfile);
}
void ExportFile::recieveFromMain(QStandardItemModel *model){
    for(int curIndex=0;curIndex<model->rowCount();curIndex++){
        QModelIndex si=model->index(curIndex,0);
        QStandardItem *curItem=model->itemFromIndex(si);
        QString childname;
        if(curItem->hasChildren()){
            int childRows=curItem->rowCount();
            childname="_"+curItem->child(childRows-1,0)->text();
        }
        QFileInfo info=QFileInfo(curItem->text());
        QString curItemName=info.baseName()+childname+"."+info.suffix();
        QStandardItem *items=new QStandardItem(curItemName);
        items->setCheckable(true);
        exmodel->setItem(curIndex,0,items);
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
    alert=new Alert();
    if(exFiles.count()==0){
        alert->setMessage("无选中导出文件！");
        alert->show();
    }else if(distpath==""){
        alert->setMessage("请选择导出路径！");
        alert->show();
    }else{
        QString temp;
        QString from=input+exFiles[0];
        QString output=distpath+"/"+exFiles[0];
        if(exFiles.count()==1){
            bool valid=QFile::copy(from,output);
            if(valid){
                alert->setMessage("导出成功！");
            }else{
                alert->setMessage("导出失败！！！");
            }
            alert->show();
        }else{
            QString type=ui->comboBox->currentText();
            QString cmd,srcfiles,outputfilename;
            if(ui->lineEdit_2->text()!=""){
                outputfilename=ui->lineEdit_2->text();
            }else{
                outputfilename=QFileInfo(exFiles[0]).baseName();
            }
            QString out=distpath+"/"+outputfilename+".tar.";
//            out=outputfilename+".tar.";
            for(int i=0;i<exFiles.count();i++){
                srcfiles+=" LDfile/"+exFiles[i];
//                qDebug()<<"from::"<<input+"/"+exFiles[i]<<"  output::"<<distpath+"/"+exFiles[i];
            }
            QProcess *pzip=new QProcess(this);
            if(type=="gz"){
                cmd="tar -zcvf "+out+type+" "+srcfiles;
            }else if(type=="bz2"){
                cmd="tar -jcvf "+out+type+" "+srcfiles;
            }
            qDebug()<<cmd;
            pzip->start(cmd);
            pzip->waitForFinished();
            QString mess=pzip->readAll();
            alert->setMessage("导出信息：\r\n"+mess);
            alert->show();
        }
    }

}


void ExportFile::on_pushButton_clicked()
{
    distpath=QFileDialog::getExistingDirectory();
    ui->lineEdit->setText(distpath);
}

