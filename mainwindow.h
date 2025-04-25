#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QLabel>
#include <QObject>
#include <QMainWindow>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui
{
  class MainWindow;
  class ClickableLabel;
}
QT_END_NAMESPACE

// 工作线程类
class Worker : public QObject
{
  Q_OBJECT
private:
  std::string crc_;
  std::string path_;

public:
  Worker(std::string crc, std::string path) : crc_(crc), path_(path) {};
public slots:
  std::string doWork();
signals:
  void workFinished(std::string);
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  std::string filePath_;
  std::vector<std::pair<std::string, std::string>> history_;

private slots:
  void openFileDialog();
  void updateHistoryLabel(std::string p, std::string res);
  void calculateCrc();
  void handleCrcResult(std::string result);
};

class ClickableLabel : public QLabel
{
  Q_OBJECT
public:
  explicit ClickableLabel(QWidget *parent = nullptr)
      : QLabel(parent)
  {
    setMouseTracking(true);
  }

signals:
  void clicked();

protected:
  void mousePressEvent(QMouseEvent *event) override
  {
    if (event->button() == Qt::LeftButton)
    {
      emit clicked();
    }
    QLabel::mousePressEvent(event);
  }
};
#endif // MAINWINDOW_H
