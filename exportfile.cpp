#include "exportfile.h"
#include "ui_exportfile.h"

ExportFile::ExportFile(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExportFile)
{
    ui->setupUi(this);
    setWindowTitle("文件导出");
}

ExportFile::~ExportFile()
{
    delete ui;
}
