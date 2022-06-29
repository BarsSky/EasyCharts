#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FileParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    void dropEvent(QDropEvent* event) override;
private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    bool initialaze();
    void makeCharts(FileParser *_parser);
};
#endif // MAINWINDOW_H
