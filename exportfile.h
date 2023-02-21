#ifndef EXPORTFILE_H
#define EXPORTFILE_H

#include <QMainWindow>

namespace Ui {
class ExportFile;
}

class ExportFile : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExportFile(QWidget *parent = nullptr);
    ~ExportFile();

private:
    Ui::ExportFile *ui;
};

#endif // EXPORTFILE_H
