#include "FileParser.h"
#include "QMessageBox"
#include "QTextStream"
#include <fstream>
#include <string>
#include <sstream>
#include <TabularChoice.h>


FileParser::FileParser()
{

}

void FileParser::openFile()
{
    fileName = QFileDialog::getOpenFileName(nullptr,
                                            tr("Open File"),
                                            ("./"),
                                            tr("Text Files (*.txt)"));
    parse();
}

void FileParser::openFile(QString _fileName)
{
    fileName = _fileName;
    parse();
}

std::vector<std::vector<float> > FileParser::getXFromFile()
{
    return x;
}

std::vector<std::vector<float> > FileParser::getYFromFile()
{
    return y;
}

std::vector<std::vector<float> > FileParser::getZFromFile()
{
    return z;
}

std::vector<QString> FileParser::getXNames()
{
    return namesX;
}

std::vector<QString> FileParser::getYNames()
{
    return namesY;
}

std::vector<QString> FileParser::getZNames()
{
    return namesZ;
}

void FileParser::parse()
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(nullptr, "Warning", "File not open!");
        return;
    }else{
        //подсичтывваем количество колон
        QTextStream in(&file);
        QString line = in.readLine();
        QStringList listOFNames = line.split("\t");
        parseAnyFile(line, fileName);
        file.close();
    }
}

void FileParser::parseAnyFile(QString line, QString _name)
{
    //сформировать окно с таблицей где выбрать необходимые столбцы для графиков
    TabularChoice* _table = new TabularChoice(line, _name);
    if (_table->exec() != QDialog::Rejected){
        x = _table->getGraphsX();
        y = _table->getGraphsY();
        z = _table->getGraphsZ();
        namesX = _table->getGraphsNames();
    }
}
