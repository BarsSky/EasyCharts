#include "ChartsDock.h"

ChartsDock::ChartsDock(QWidget *parent): QMainWindow(parent)
{
    dock = nullptr;
}

ChartsDock::~ChartsDock()
{
    delete dock;
}

void ChartsDock::addWidgetToDock(QWidget *widget, QString nameChart)
{
    dock = new QDockWidget(nameChart, this);
    //dock->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
    dock->setFocusPolicy(Qt::NoFocus);
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    dock->setFloating(false);
    dock->setWidget(widget);

    addDockWidget(Qt::LeftDockWidgetArea,dock);
}

void ChartsDock::addWidgetsToDock(std::vector<QWidget *> widget, std::vector<QString> nameChart)
{
    for(unsigned int i = 0; i < widget.size(); i++)
    {
        dock = new QDockWidget(nameChart.at(i), this);
        //dock->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);

        dock->setFeatures(QDockWidget::DockWidgetMovable);
        dock->setFloating(false);
        dock->setWidget(widget.at(i));

        addDockWidget(i > (widget.size()/2 - 1)? Qt::RightDockWidgetArea : Qt::LeftDockWidgetArea,dock);
    }
}
