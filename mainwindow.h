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
    QStandardItemModel *model;
    QString homePath,loadpath,loadfilename;

private slots:
    void on_actiondaoru_triggered();

    void on_actiondaochu_triggered();

    void on_action_triggered();

    void on_actiontuxiang_triggered();

    void on_actiontuxiang_2_triggered();

    void on_action_2_triggered();

    void on_actionduibi_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QPointF lastpoint;
    qreal scale=1;
    ExportFile *ef;
    ShowRes *res;
    QString onclicked;
    QPixmap *currentPix;
    QByteArray NUL;
    int re;
    QString t;
    QImage mirrored,pre;
    ConRatio *ratio;
    Alert *alertDia;
};
#endif // MAINWINDOW_H
