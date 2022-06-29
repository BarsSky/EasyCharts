#ifndef CHARTSDOCK_H
#define CHARTSDOCK_H

#include <QWidget>
#include "QMainWindow"
#include "QtWidgets"
#include "QDockWidget"

class ChartsDock: public QMainWindow
{
public:
    explicit ChartsDock(QWidget *parent = nullptr);
    ~ChartsDock();
    void addWidgetToDock(QWidget * widget, QString nameChart);
    void addWidgetsToDock(std::vector<QWidget *> widget, std::vector<QString> nameChart);
private:
    QDockWidget *dock;
};

#endif // CHARTSDOCK_H
