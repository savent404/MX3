#pragma once

#include "iParam.h"
#include "ff.h"

class pParam : public iParam
{
public:
    pParam(int bank);
    /**
     * @name port func
     * @{ */
    virtual int searchFileCnt(const char* path, const char* regex) const;
    virtual bool searchFileName(const char* path, const char* regex, std::string& out, int pos) const;
    virtual bool readConfigFromFile(const char* filepath);
    virtual bool readColorConfigFromFile(const char* filepath);
    virtual bool readStaticParameter();
    virtual bool writeStaticParameter();
};
