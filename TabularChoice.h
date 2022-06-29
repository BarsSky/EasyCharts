#ifndef TABULARCHOICE_H
#define TABULARCHOICE_H

#include <QDialog>
#include <QObject>
#include <QSet>
#include <QTableWidget>
#include <QFile>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <set>
#include <list>

struct Graph{
public:
    Graph(std::vector<float> x, std::vector<float> y, std::vector<float> z = std::vector<float>(),QString name = "Graph"){
        _x = x;
        _y = y;
        _z = z;
    }
    std::vector<float> getX(){
        return _x;
    }
    std::vector<float> getY(){
        return _y;
    }
    std::vector<float> getZ(){
        return _z;
    }
    QString getName(){
        return _name;
    }
    void setName(QString name){
        _name = name;
    }
private:
    //запоминаем массивы X и Y
    std::vector<float> _x, _y, _z;
    QString _name;
};

class TabularChoice: public QDialog
{
    Q_OBJECT
public:
    explicit TabularChoice(QString line, QString _name, QWidget* parent = nullptr);
    std::vector<std::vector<float>> getGraphsX();
    std::vector<std::vector<float>> getGraphsY();
    std::vector<std::vector<float>> getGraphsZ();
    std::vector<QString> getGraphsNames();
private:
    void makeUI();
    void fillTable();
    void createSignals();
    void setFromTable();
    //
    QGridLayout* _layout;
    QTableWidget* new_table;
    QListWidget* new_list;
    QPushButton* new_button_Add;
    QPushButton* new_button_Ok;
    QPushButton* new_button_Cancel;
    //
    QString m_fileName;
    //
    QSet<unsigned int> selected_column;
    std::list<Graph*> graphList;
    std::vector<float> _x, _y, _z;
public slots:
    void selectedColumn(int r, int c);
    void addGraph();
    void acceptGraphs();
    void rejectGraph();
};

#endif // TABULARCHOICE_H
