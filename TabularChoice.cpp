#include "TabularChoice.h"
#include <fstream>
#include <string>
#include <sstream>
#include <QMessageBox>

TabularChoice::TabularChoice(QString line, QString _name, QWidget *parent) :
    QDialog(parent),
    m_fileName(_name)
{
    makeUI();
    createSignals();
    //Fill column
    QStringList _list = line.split("\t");
    if(_list.back() == "")
        _list.erase(_list.begin() + _list.size()-1);
    new_table->setColumnCount(_list.size());
    new_table->setHorizontalHeaderLabels(_list);
    //Fill table
    fillTable();
}

std::vector<std::vector<float> > TabularChoice::getGraphsY()
{
    std::vector<std::vector<float>> Y;
    for(auto graph : graphList)
        Y.emplace_back(graph->getY());
    return Y;
}

std::vector<std::vector<float> > TabularChoice::getGraphsZ()
{
    std::vector<std::vector<float>> Z;
    for(auto graph : graphList)
        Z.emplace_back(graph->getZ());
    return Z;
}

std::vector<QString> TabularChoice::getGraphsNames()
{
    std::vector<QString> names;
    for(auto graph : graphList)
        names.emplace_back(graph->getName());
    return names;
}

std::vector<std::vector<float>> TabularChoice::getGraphsX()
{
    std::vector<std::vector<float>> X;
    for(auto graph : graphList)
        X.emplace_back(graph->getX());
    return X;
}

void TabularChoice::makeUI()
{
    //make UI
    _layout = new QGridLayout();
    QVBoxLayout* vLayout1 = new QVBoxLayout();
    QVBoxLayout* vLayout2 = new QVBoxLayout();
    QHBoxLayout* hLayout1 = new QHBoxLayout();
    new_table = new QTableWidget();
    new_list = new QListWidget();
    new_button_Ok = new QPushButton();
    new_button_Cancel = new QPushButton();
    new_button_Add = new QPushButton();
    //
    vLayout1->addWidget(new_button_Add,0);
    vLayout1->addWidget(new_list,1);
    //
    vLayout2->addWidget(new_table,0);
    hLayout1->addWidget(new_button_Ok,0);
    hLayout1->addWidget(new_button_Cancel,1);
    hLayout1->setAlignment(Qt::AlignRight);
    vLayout2->addLayout(hLayout1,1);
    _layout->addLayout(vLayout1,0,0);
    _layout->addLayout(vLayout2,0,1);

    new_button_Add->setText(QString("Add"));
    new_button_Add->setGeometry(QRect(0,0,40,20));
    new_button_Add->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    new_button_Add->setDisabled(true);
    //
    new_list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //
    new_button_Ok->setText(QString("Ok"));
    new_button_Ok->setGeometry(QRect(0,0,40,20));
    new_button_Ok->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    new_button_Ok->setDisabled(true);
    //
    new_button_Cancel->setText(QString("Cancel"));
    new_button_Cancel->setGeometry(QRect(0,0,40,20));
    new_button_Cancel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    setVisible(true);
    setLayout(_layout);
    _layout->setColumnStretch(0,0);
    _layout->setColumnStretch(1,4);
    _layout->setSpacing(2);
    setBaseSize(640,480);
}

void TabularChoice::fillTable()
{
    new_table->setSelectionMode(QAbstractItemView::MultiSelection);
    new_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    new_table->setSelectionBehavior(QAbstractItemView::SelectColumns);
    std::vector<double> fileDoubleStorage;
    std::string str;
    std::ifstream fileIn(m_fileName.toLocal8Bit().data());
    if (fileIn.is_open())
    {
        while (std::getline(fileIn, str))
        {
            std::istringstream ss(str);
            double value;
            while (ss >> value)
                fileDoubleStorage.emplace_back(value);
        }
        fileIn.close();
    }
    size_t count = 0;
    new_table->setRowCount(new_table->rowCount() + 1);
    for(auto _element : fileDoubleStorage){
        QTableWidgetItem* _item = new QTableWidgetItem(QString::number(_element));
        new_table->setItem(new_table->rowCount() - 1, count, _item);
        if(count > static_cast<size_t>(new_table->columnCount()-2)){
            count = 0;
            new_table->setRowCount(new_table->rowCount() + 1);
        }
        else
            count++;

    }
    new_table->removeRow(new_table->rowCount()-1);
    //
}

void TabularChoice::createSignals()
{
    connect(this->new_table,SIGNAL(cellClicked(int,int)),this,SLOT(selectedColumn(int,int)));
    connect(this->new_button_Add, SIGNAL(pressed()),this,SLOT(addGraph()));
    connect(this->new_button_Ok, SIGNAL(pressed()),this,SLOT(acceptGraphs()));
    connect(this->new_button_Cancel, SIGNAL(pressed()),this,SLOT(rejectGraph()));
}

void TabularChoice::selectedColumn(int r, int c)
{
    Q_UNUSED(r)
    //remember column mark all column
    if(selected_column.size() < 2){
        if(!selected_column.contains(c)){
            selected_column.insert(c);
            new_button_Add->setEnabled(true);
        }
        else{
            QSet<unsigned int>::iterator _iter = selected_column.find(c);
            selected_column.erase(_iter);
            if(selected_column.size() == 0)
                new_button_Add->setEnabled(false);
        }
        //
    }
    else{
        QMessageBox::warning(this, "Warning", "Can build only 2D graph.\n Please chek one or two columns.");
    }
}

void TabularChoice::addGraph()
{
    //add to List
    new_list->addItem("Graph");
    setFromTable();
    graphList.emplace_back(new Graph(_x,_y,_z));
    new_button_Ok->setEnabled(true);
    //
    selected_column.clear();
}

void TabularChoice::acceptGraphs()
{
    accept();
}

void TabularChoice::rejectGraph()
{
    reject();
}

void TabularChoice::setFromTable()
{
    _x.clear();
    _y.clear();
    _z.clear();
    if(selected_column.size() > 1){
        unsigned int rCount = new_table->rowCount();
        unsigned int _buf = 0;
        for(auto _column : selected_column){
            for(unsigned int i = 0; i < rCount; i++){
                if(_buf == 0)
                    _x.emplace_back(new_table->item(i,_column)->text().toFloat());
                if(_buf == 1)
                    _y.emplace_back(new_table->item(i,_column)->text().toFloat());
                if(_buf == 2)
                    _z.emplace_back(new_table->item(i,_column)->text().toFloat());
            }
            _buf++;
        }
    }else{
        unsigned int rCount = new_table->rowCount();
        for(auto _column : selected_column){
            for(unsigned int i = 0; i < rCount; i++){
                _x.emplace_back(i);
                _y.emplace_back(new_table->item(i,_column)->text().toFloat());
            }
        }
    }
}
