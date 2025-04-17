#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class ClickableLabel;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::string filePath_;

private slots:
    void openFileDialog();
    std::string calculateCrc();
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
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }
};
#endif // MAINWINDOW_H
