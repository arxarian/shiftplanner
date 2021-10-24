#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sources/workers/workermodel.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void AddOperators();

private:
    Ui::MainWindow* ui;

    WorkerModel* m_workers = nullptr;
};
#endif // MAINWINDOW_H
