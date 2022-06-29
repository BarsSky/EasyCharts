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
//        if(listOFNames.at(0) == "aix"){
//            coordFile(line);
//            file.close();
//            return;
//        }
//        if(listOFNames.at(0) == "rr[1]"){
//            rrFile(line);
//            file.close();
//            return;
//        }
//        if(listOFNames.at(0) == "del1"){
//            delFile(line);
//            file.close();
//            return;
//        }
//        if(listOFNames.at(0) == "sok_g1[1]"){
//            rrFile(line);
//            file.close();
//            return;
//        }
//        if(listOFNames.at(0) == "vix"){
//            coordFile(line);
//            file.close();
//            return;
//        }
//        if(listOFNames.at(0) == "MASSA"){
//            delFile(line);
//            file.close();
//            return;
//        }
//        if(listOFNames.at(0) == "h"){
//            hFile(line);
//            file.close();
//            return;
//        }
        parseAnyFile(line, fileName);
        file.close();
    }
}

void FileParser::coordFile(QString line)
{
    int columns = line.count("\t");
    int xcolumn = line.count("x");
    int ycolumn = line.count("y");
    int zcolumn = line.count("z");
    QStringList listOFNames = line.split("\t");
    int count = 0;
    for(auto string : listOFNames){
        if(count == 0){
            namesX.emplace_back(string);
            count++;
            continue;
        }
        if(count == 1){
            namesY.emplace_back(string);
            count++;
            continue;
        }
        if(count == 2){
            namesZ.emplace_back(string);
            count = 0;
            continue;
        }

    }
    //считываем все цифровые значения
    std::vector<double> fileDoubleStorage;
    std::string str;
    std::ifstream fileIn(fileName.toLocal8Bit().data());
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
    //переносим полученные значения по массивам
    count  = 0;
    bool beX = true,beY = false,beZ = false;
    x.resize(xcolumn);
    y.resize(ycolumn);
    z.resize(zcolumn);
    for(auto element : fileDoubleStorage){
        if(beX){
            x.at(count).emplace_back(element);
            beX = false;
            beY = true;
        }
        if(beY){
            y.at(count).emplace_back(element);
            beY = false;
            beZ = true;
        }
        if(beZ){
            z.at(count).emplace_back(element);
            beZ = false;
            beX = true;
            if(count < xcolumn-1)
                count++;
            else
                count = 0;
        }
    }
}

void FileParser::rrFile(QString line)
{
    int columns = line.count("\t");
    int xcolumn = line.count("[1]");
    int ycolumn = line.count("[2]");
    int zcolumn = line.count("[3]");
    QStringList listOFNames = line.split("\t");
    int count = 0;
    for(auto string : listOFNames){
        if(count == 0){
            namesX.emplace_back(string);
            count++;
            continue;
        }
        if(count == 1){
            namesY.emplace_back(string);
            count++;
            continue;
        }
        if(count == 2){
            namesZ.emplace_back(string);
            count = 0;
            continue;
        }

    }
    //считываем все цифровые значения
    std::vector<double> fileDoubleStorage;
    std::string str;
    std::ifstream fileIn(fileName.toLocal8Bit().data());
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
    //переносим полученные значения по массивам
    count  = 0;
    bool beX = true,beY = false,beZ = false;
    x.resize(xcolumn);
    y.resize(ycolumn);
    z.resize(zcolumn);
    for(auto element : fileDoubleStorage){
        if(beX){
            x.at(count).emplace_back(element);
            beX = false;
            beY = true;
        }
        if(beY){
            y.at(count).emplace_back(element);
            beY = false;
            beZ = true;
        }
        if(beZ){
            z.at(count).emplace_back(element);
            beZ = false;
            beX = true;
            if(count < xcolumn-1)
                count++;
            else
                count = 0;
        }
    }
}

void FileParser::delFile(QString line)
{
    int columns = line.count("\t");
    QStringList listOFNames = line.split("\t");
    int count = 0;
    for(auto string : listOFNames){
        namesX.emplace_back(string);
    }
    //считываем все цифровые значения
    std::vector<double> fileDoubleStorage;
    std::string str;
    std::ifstream fileIn(fileName.toLocal8Bit().data());
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
    //переносим полученные значения по массивам
    count  = 0;
    bool beX = false, beY = true;
    x.resize(columns);
    y.resize(columns);
    for(auto element : fileDoubleStorage){
        if(beY){
            y.at(count).emplace_back(element);

            count++;
            if(count > columns-1){
                beY = false;
                beX = true;
                count = 0;
            }
        }
        if(beX){
            for(size_t i = 0 ; i < x.size(); i++){
                x.at(i).emplace_back(element);
            }
            beX = false;
            beY = true;
        }
    }
}

void FileParser::hFile(QString line)
{
    int columns = line.count("\t");
    QStringList listOFNames = line.split("\t");
    int count = 0;

    namesX.emplace_back("Time");
    namesY.emplace_back("h");

    //считываем все цифровые значения
    std::vector<double> fileDoubleStorage;
    std::string str;
    std::ifstream fileIn(fileName.toLocal8Bit().data());
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
    //переносим полученные значения по массивам
    bool beX = true, beY = false;
    x.resize(1);
    y.resize(1);
    for(auto element : fileDoubleStorage){
        if(beX){
            x.at(count).emplace_back(element);
            beX = false;
            beY = true;
        }
        if(beY){
            y.at(count).emplace_back(element);
            beY = false;
            beX = true;
        }
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
