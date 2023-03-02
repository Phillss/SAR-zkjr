#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QElapsedTimer>
#include <QScreen>
#include <QRect>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScale>
#include <QWheelEvent>
#include <QMouseEvent>
#include "image_scaled_widget.h"
#include "exportfile.h"
#include "showres.h"
#include <QMatrix>
#include "conratio.h"
#include "alert.h"
#include <QtXml>
#include <QDomDocument>
#include "threshold.h"
#include <QHash>
#include "algom.h"
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct SarfileList{
    QString filename;
    QList<QString> historyModiy;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void select(const QModelIndex &index);
    void algori_select(const QModelIndex &index);
    void loadXML(QString xmlpath);
    QImage AdjustContrast(QImage Img, int iContrastValue)
    {
        int pixels = Img.width() * Img.height();
        unsigned int *data = (unsigned int *)Img.bits();

        int red, green, blue, nRed, nGreen, nBlue;

        if (iContrastValue > 0 && iContrastValue < 100)
        {
            float param = 1 / (1 - iContrastValue / 100.0) - 1;

            for (int i = 0; i < pixels; ++i)
            {
                nRed = qRed(data[i]);
                nGreen = qGreen(data[i]);
                nBlue = qBlue(data[i]);

                red = nRed + (nRed - 127) * param;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - 127) * param;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - 127) * param;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
        }
        else
        {
            for (int i = 0; i < pixels; ++i)
            {
                nRed = qRed(data[i]);
                nGreen = qGreen(data[i]);
                nBlue = qBlue(data[i]);

                red = nRed + (nRed - 127) * iContrastValue / 100.0;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - 127) * iContrastValue / 100.0;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - 127) * iContrastValue / 100.0;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
        }

        return Img;
    }
    void setContrast(int value);
    QStandardItemModel *model,*algorithmmodel;
    QString homePath,loadpath,loadfilename;
    void onReadData();

signals:
    void sendToExport(QStandardItemModel *model);

public slots:
    void recieveValue(int value);

private slots:
    void on_actiondaoru_triggered();

    void on_actiondaochu_triggered();

    void on_action_triggered();

    void on_actiontuxiang_triggered();

    void on_actiontuxiang_2_triggered();

    void on_action_2_triggered();

    void on_actionduibi_triggered();
    void listDom(QDomElement& docElem);
    void listAlgorihm(QDomNode& algorithmNodes);

    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_textEdited(const QString &arg1);
    void appendAlgorithmResult(QString distname,QString pathname);

protected:
    Ui::MainWindow *ui;
    QString onStartPic,onStartHeader,mainWindowHeader,algorithmpath,algorithmlistname,filelist,output;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPointF lastpoint;
    qreal scale=1;
    ExportFile *ef;
    ShowRes *res;
    QString onclicked,t;//
    QImage pre,currImg;//
    QPixmap currentPix;
    QByteArray NUL;
    int re;
    ConRatio *ratio;
    Alert *alertDia;
    QStringList algorithmHeader;
    threshold *holder;
    QDomElement docelement;
    QHash <int,Algom*> hash;
    QHash<QString,int> nameToindex;
    int onclickedRow=-1;
    QString nullmessage;
    QString suff;

};
#endif // MAINWINDOW_H
