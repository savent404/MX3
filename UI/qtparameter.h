#ifndef QTPARAMETER_H
#define QTPARAMETER_H

#include "iParam.h"
#include <QDir>

class qtParameter : public iParam {
public:
    virtual int searchFileCnt(const char* path, const char* regex) const;
    virtual bool searchFileName(const char* path, const char* regex, std::string& out, int pos) const;
    virtual bool readConfigFromFile(const char* filepath);
    virtual bool readColorConfigFromFile(const char* filepath);
    virtual bool readStaticParameter();
    virtual bool writeStaticParameter();

public:
    qtParameter(int bankPos);

    ~qtParameter();
};

#endif // QTPARAMETER_H
