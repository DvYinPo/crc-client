#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QThread>
#include <QString>
#include <QTimeLine>
#include <QMessageBox>
#include <iostream>
#include "crc.h"

std::string Worker::doWork()
{
  CRC calculatorCRC64(this->crc_);
  std::string result = calculatorCRC64.file(this->path_).dec();
  emit Worker::workFinished(result);
  return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->statusBar()->setSizeGripEnabled(false);

  std::vector<std::string> list = CRC::getPoly();
  for (auto item : list)
  {
    ui->selectCRC->addItem(QString::fromStdString(item));
  }

  connect(ui->labelPath, &ClickableLabel::clicked, this, &MainWindow::openFileDialog);
  connect(ui->btnCalculate, &QPushButton::clicked, this, &MainWindow::calculateCrc);
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

void MainWindow::updateHistoryLabel(std::string p, std::string res)
{
  this->history_.push_back({p, res});
  std::string historyRes = "";
  for (size_t i = 0; i < this->history_.size(); i++)
  {
    const auto &[path, result] = this->history_[i];
    historyRes += std::to_string(i + 1) + ".  " + path + "\t" + result + "\n";
  }

  ui->labelHistory->setText(QString::fromStdString(historyRes));
}

void MainWindow::calculateCrc()
{
  if (this->filePath_.empty())
  {
    QMessageBox::critical(this, "计算错误", "请先选择文件！");
    return;
  }
  if (ui->selectCRC->currentText().isEmpty())
  {
    QMessageBox::critical(this, "计算错误", "请先选择CRC算法！");
    return;
  }
  ui->btnCalculate->setEnabled(false);
  auto userSelectedCrc = ui->selectCRC->currentText();

  QTimeLine *timeLine = new QTimeLine(1000, this);
  timeLine->setFrameRange(0, 4);
  timeLine->setLoopCount(0);
  connect(timeLine, &QTimeLine::frameChanged, this, [this](int frame)
          {
    QString text = "calculating" + QString(".").repeated(frame);
    ui->labelResult->setText(text); });

  QThread *thread = new QThread;
  Worker *worker = new Worker(userSelectedCrc.toStdString(), this->filePath_);
  worker->moveToThread(thread);

  connect(thread, &QThread::started, worker, &Worker::doWork);
  connect(worker, &Worker::workFinished, this, [this, timeLine](std::string result)
          {
            timeLine->stop();
            this->handleCrcResult(result);
            this->ui->btnCalculate->setEnabled(true); });
  connect(worker, &Worker::workFinished, worker, &QObject::deleteLater);
  connect(thread, &QThread::finished, worker, &Worker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  timeLine->start();
  thread->start();
}

void MainWindow::handleCrcResult(std::string result)
{
  this->ui->labelResult->setText(QString::fromStdString(result));
  this->updateHistoryLabel(this->filePath_, result);
}
