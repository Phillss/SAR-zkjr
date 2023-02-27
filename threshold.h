#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <QDialog>
#include <QProcess>
#include <QFile>
#include <QFileDialog>
#include <QStandardItemModel>

namespace Ui {
class threshold;
}

class threshold : public QDialog
{
    Q_OBJECT

public:
    explicit threshold(QWidget *parent = nullptr);
    ~threshold();
    void setMaxt(int t);
    void setMint(int t);
    void setDefault(int t);
    void setMaxH(int h);
    void setMaxW(int w);
    void setFalg(bool flag);
    void setFilename(QString name);
    void setalgorithmpath(QString name);
    void setalgorithmDirName(QString name);
    void setAlgorithmName(QString name);
    void setDistname(QString name);
    void setAlgorithmCn(QString name);

private slots:

    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::threshold *ui;
    QString filename,processPath,algorithmpath,algorithmDirName,algorithmName,profileDir,distname,outputdir,algorithmnameCn;
    int max_t,min_t,default_t,max_height=10000,max_width=10000;
    bool flag;
    QStandardItemModel *showModel;
};

#endif // THRESHOLD_H
