#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SkyChart_base.h"
#include "ChartsDock.h"
#include "QMessageBox"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);
    if(!initialaze())
        QMessageBox::warning(this,"Warning","Not initialize!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void createGraf(std::vector<float> *_x, std::vector<float> *_y){
    _x->emplace_back();
    _y->emplace_back();
    for(int i = -100; i < 100; i++)
    {
        _x->emplace_back(i);
        _y->emplace_back(sin(_x->back())+(std::rand()%100)*tan(_x->back()) + cos(pow(_x->back(),std::rand()%10)));
    }
}

bool MainWindow::initialaze()
{
    std::vector<QString> text;
    std::vector<float> x,y;
    std::vector<QWidget*> _chartArray;
    text.resize(2,"Graph");//3
    SkyChart_base* sky_chart = new SkyChart_base();
    SkyChart_base* sky_chart2 = new SkyChart_base();
    createGraf(&x,&y);
    sky_chart->createOrAdd2DChart(&x,&y);
    //
    createGraf(&x,&y);
    sky_chart2->createOrAdd2DChart(&x,&y);
    //
    _chartArray.emplace_back(sky_chart);
    _chartArray.emplace_back(sky_chart2);
    //add chart
    ChartsDock* _chDock = new ChartsDock();
    _chDock->addWidgetsToDock(_chartArray,text);
    ui->gridLayout->addWidget(_chDock,0,0);
    return true;
}

void MainWindow::makeCharts(FileParser* _parser)
{
    std::vector<QWidget*> _chartArray;
    for(size_t column = 0; column < _parser->getXFromFile().size(); column++){
        SkyChart_base* sky_chart = new SkyChart_base();
        sky_chart->createOrAdd2DChart(&_parser->getXFromFile().at(column),&_parser->getYFromFile().at(column));
        _chartArray.emplace_back(sky_chart);
    }
    ChartsDock* _chDock = new ChartsDock();
    _chDock->addWidgetsToDock(_chartArray,_parser->getXNames());
    ui->gridLayout->addWidget(_chDock,0,0);
}


void MainWindow::on_actionOpen_triggered()
{
    delete ui->gridLayout->takeAt(0)->widget();
    FileParser* _parser = new FileParser();
    _parser->openFile();
    makeCharts(_parser);
}
