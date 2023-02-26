#include "mainwindow.h"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QFile>
#include <QFileDialog>
#include "loadfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen * splash=new QSplashScreen;
    LoadFile* load=new LoadFile();
    QString iniFilePath=QCoreApplication::applicationDirPath()+"/ini.xml";
    load->loadXML(iniFilePath);
    QPixmap onstart(":/res/"+load->getOnstartpic());
    QString name=load->getOnstartHeader();
    splash->showMessage(name,Qt::AlignBottom|Qt::AlignCenter,Qt::gray);
    splash->setFont(QFont("microsoft yahei",30,QFont::Bold));
    splash->setPixmap(onstart);
    splash->show();
    a.processEvents();

    MainWindow w;
    w.show();
    splash->finish(&w);
    delete splash;
    return a.exec();
}
