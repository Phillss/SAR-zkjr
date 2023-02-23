#ifndef EXPORTFILE_H
#define EXPORTFILE_H

#include <QDialog>
#include <QStandardItemModel>
#include <qdebug.h>

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

public slots:
    void recieveFromMain(QStandardItemModel *model);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExportFile *ui;
};

#endif // EXPORTFILE_H
