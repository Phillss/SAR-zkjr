#ifndef EXPORTFILE_H
#define EXPORTFILE_H

#include <QDialog>
#include <QStandardItemModel>
#include <qdebug.h>
#include <QPushButton>
#include "alert.h"
#include <QFile>
#include <QFileDialog>

namespace Ui {
class ExportFile;
}

class ExportFile : public QDialog
{
    Q_OBJECT

public:
    explicit ExportFile(QWidget *parent = nullptr);
    ~ExportFile();
    QStandardItemModel *exmodel;
    void exportFiles(QStandardItemModel *model);
signals:
    void exsign(QStandardItemModel* item);
public slots:
    void recieveFromMain(QStandardItemModel *model);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExportFile *ui;
    Alert *alert;
    QList<QString> exFiles;
    QString output;
};

#endif // EXPORTFILE_H
