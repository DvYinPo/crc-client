#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->statusBar()->setSizeGripEnabled(false);

  connect(ui->labelPath, &ClickableLabel::clicked, this, &MainWindow::openFileDialog);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::openFileDialog()
{
  // 弹出文件选择对话框
  QString filePath = QFileDialog::getOpenFileName(this,
                                                  "选择文件",
                                                  QDir::homePath(),
                                                  "所有文件 (*)");

  if (!filePath.isEmpty())
  {
    this->filePath_ = filePath.toStdString();
    ui->labelPath->setText(filePath);
  }
}

std::string MainWindow::calculateCrc()
{
  // CRC calculatorCRC64("crc-64-ECMA");

  // std::string result = calculatorCRC64.file(this->filePath_).dec();
  // this->ui->labelResult->setText(QString::fromStdString(result));
  // return result;
  return "";
}
