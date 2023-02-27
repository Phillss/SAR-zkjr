#include "loadfile.h"

LoadFile::LoadFile()
{
    alertDia=new Alert();
}
QString LoadFile::getOnstartpic(){
    return onstartpic;
}
QString LoadFile::getOnstartHeader(){
    return onstartheader;
}
QString LoadFile::getMainwindow(){
    return mainwindowheader;
}
QString LoadFile::getAlgorithm(){
    return algorithmpath;
}
QString LoadFile::getlistname(){
    return algorithmlistname;
}
QString LoadFile::getfilelist(){
    return filelist;
}
void LoadFile::loadXML(QString xmlpath){
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
        QDomElement docelement=doc.documentElement();
        listDom(docelement);
    }
}
//解析配置文件
void LoadFile::listDom(QDomElement& docElem)
{
    QDomNode node = docElem.firstChild();
    QString tagname;
    if(node.toElement().isNull()){
        return;
    }
    while(!node.isNull()){
        tagname=node.toElement().tagName();
        if(tagname=="onstartpic"){
            this->onstartpic=node.toElement().text();
        }else if(tagname=="onstartheader"){
            this->onstartheader=node.toElement().text();
        }else if(tagname=="mainwindowheader"){
            this->mainwindowheader=node.toElement().text();
        }else if(tagname=="algorithmpath"){
            this->algorithmpath=node.toElement().text();
        }else if(tagname=="algorithmlistname"){
            this->algorithmlistname=node.toElement().text();
        }else if(tagname=="filelistname"){
            this->filelist=node.toElement().text();
        }
        node=node.nextSibling();
    }
}
