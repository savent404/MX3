#include "qtparameter.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <string>

using namespace std;
string convert_upper(string& c)
{
    string v(c);
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] <= 'z' && v[i] >= 'a')
            v[i] -= 32;
    }
    return v;
}

int qtParameter::searchFileCnt(const char* path, const char* regex) const
{
    QString qpath = QString(path);
    QDir dir(qpath);
    const QFileInfoList list = dir.entryInfoList();
    int cnt = 0;
    for (int i = 0; i < list.size(); i++) {
        string name = list[i].fileName().toUtf8().constData();
        string fileName = convert_upper(name);
        if (!re_match(regex, fileName.c_str())) {
            cnt++;
        }
    }
    return cnt;
}

bool qtParameter::searchFileName(const char* path, const char* regex, std::string& out, int pos) const
{
    QString qpath = QString(path);
    QDir dir(qpath);
    const QFileInfoList list = dir.entryInfoList();
    int cnt = 0;
    for (int i = 0; i < list.size(); i++) {
        string name = list[i].fileName().toUtf8().constData();
        string fileName = convert_upper(name);
        if (!re_match(regex, fileName.c_str())) {
            if (cnt++ >= pos) {
                out = name;
                return true;
            }
        }
    }
    return false;
}

bool qtParameter::readConfigFromFile(const char* filepath)
{
    QString filePath(filepath);
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QIODevice::Text))
        return false;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        string str = QString(line).toUtf8().constData();
        setParameterFromLine(str.c_str());
    }
    file.close();
    return true;
}

bool qtParameter::readColorConfigFromFile(const char* filepath)
{
    QString filePath = QString(filepath);
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly | QIODevice::Text))
        return false;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        string str = QString(line).toUtf8().constData();
        setColorParameterFromLine(str.c_str());
    }
    file.close();
    return true;
}

qtParameter::qtParameter(int bankPos)
    : iParam(bankPos)
{
    /** Search Alivable BANs */
    QString str = QFileDialog::getExistingDirectory(0, QString(), QString("~/Documents/MX_Unity")) + "/";
    workPath = str.toUtf8().constData();
    numBank = searchFileCnt(workPath.c_str(), "BANK*");

    intParam.resize(sizeof(typeIntParam) / sizeof(string));
    floatParam.resize(sizeof(typeFloatParam) / sizeof(string));

    /** read static parameter */
    if (readStaticParameter() && (size_t(getBankNum()) == staticParam.configRGBIndex.size())) {
        // remain the old config
        posBank = staticParam.posBank;
    } else {
        size_t rgbNum = sizeof(typeRGBParam) / sizeof(string);
        staticParam.configRGBIndex.reserve(getBankNum());
        staticParam.configRGB.reserve(getBankNum() * rgbNum);
        staticParam.configRGBIndex.resize(getBankNum());
        posBank = bankPos;
    }

    /** Init Color Martrix */
    if (!readColorConfigFromFile((workPath + "COLORMATRIX.txt").c_str())) {
        qDebug() << "Open color matrix.txt error";
    }

    switchBank(posBank);

    inited = true;
}

qtParameter::~qtParameter()
{
    writeStaticParameter();
}

bool qtParameter::readStaticParameter()
{
    string path = workPath + "backup.b";
    QString qPath = QString(path.c_str());
    QFile file(qPath);
    int num_bank, num_rgb;

    staticParam.configRGB.clear();
    staticParam.configRGBIndex.clear();
    if (!file.exists())
        return false;
    if (!file.open(QFile::ReadOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    size_t RGBNum = sizeof(typeRGBParam) / sizeof(string);

    out >> staticParam.posBank >> num_rgb >> num_bank;

    for (size_t i = 0; i < num_rgb * RGBNum; i++) {
        int r, g, b;
        out >> r >> g >> b;
        RGB rgb(r, g, b);
        staticParam.configRGB.push_back(rgb);
    }
    for (int i = 0; i < num_bank; i++) {
        int a;
        out >> a;
        staticParam.configRGBIndex.push_back(a);
    }

    file.close();
    return true;
}

bool qtParameter::writeStaticParameter()
{
    string path = workPath + "backup.b";
    QString qPath = QString(path.c_str());
    QFile file(qPath);

    if (!file.open(QFile::WriteOnly | QIODevice::Text))
        return false;

    file.seek(0);
    QTextStream in(&file);
    in << staticParam.posBank << " "
       << staticParam.configRGB.size() << " "
       << staticParam.configRGBIndex.size() << "\r\n";

    for (size_t i = 0; i < staticParam.configRGB.size(); i++) {
        in << int(staticParam.configRGB[i].R) << " "
           << int(staticParam.configRGB[i].G) << " "
           << int(staticParam.configRGB[i].B) << " ";
    }

    in << "\r\n";

    for (size_t i = 0; i < staticParam.configRGBIndex.size(); i++) {
        in << staticParam.configRGBIndex[i] << " ";
    }
    file.close();
    return true;
}
