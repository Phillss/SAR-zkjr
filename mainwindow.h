#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
