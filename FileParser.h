#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "QFileDialog"
#include "QFile"
#include "QDir"
#include "QObject"
#include <vector>

class FileParser: QObject
{
    Q_OBJECT
public:
    FileParser();
    void openFile();
    void openFile(QString _fileName);
    std::vector<std::vector<float>> getXFromFile();
    std::vector<std::vector<float>> getYFromFile();
    std::vector<std::vector<float>> getZFromFile();

    std::vector<QString> getXNames();
    std::vector<QString> getYNames();
    std::vector<QString> getZNames();
private:
    QString fileName;
    void parse();
    void coordFile(QString line);
    void rrFile(QString line);
    void delFile(QString line);
    void sokFile(QString line);
    void vixFile(QString line);
    void MassaFile(QString line);
    void hFile(QString line);
    void parseAnyFile(QString line, QString _name);

    std::vector<std::vector<float>> x,y,z;
    std::vector<QString> namesX,namesY,namesZ;
};

#endif // FILEPARSER_H
