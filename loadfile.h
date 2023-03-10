#ifndef LOADFILE_H
#define LOADFILE_H
#include <QString>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QTreeView>
#include <QFile>
#include "algom.h"
#include <QHash>
#include "alert.h"

class LoadFile
{
public:
    LoadFile();
    void loadXML(QString xmlpath);
    void listDom(QDomElement& docElem);
    QString getOnstartpic();
    QString getOnstartHeader();
    QString getMainwindow();
    QString getAlgorithm();
    QString getlistname();
    QString getfilelist();

private:
    Alert *alertDia;
    QString onstartpic,onstartheader,mainwindowheader,algorithmpath,algorithmlistname,filelist;
};

#endif // LOADFILE_H
