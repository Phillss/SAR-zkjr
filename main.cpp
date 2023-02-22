#include "mainwindow.h"

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QFile>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap onstart(":/res/onstart01.png");
//    QSplashScreen * splash=new QSplashScreen;
//    splash->showMessage(QStringLiteral("SAR图像地面站辅助判读系统"),Qt::AlignBottom|Qt::AlignCenter,Qt::gray);
//    splash->setFont(QFont("microsoft yahei",30,QFont::Bold));
//    splash->setPixmap(onstart);
//    splash->show();
//    a.processEvents();

    MainWindow w;
    w.show();
//    splash->finish(&w);
//    delete splash;
    return a.exec();
}
