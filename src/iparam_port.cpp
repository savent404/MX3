#include "iparam_port.h"
#include <string>

using namespace std;

pParam::pParam(int bank) : iParam(bank)
{
    workPath = "0:/";
    numBank = searchFileCnt(workPath.c_str(), "[Bb][Aa][Nn][Kk]*");
    
    intParam.resize(sizeof(typeIntParam) / sizeof(string));

    /** Read static parameter */
    if (readStaticParameter() && (size_t(getBankNum()) == staticParam.configRGBIndex.size())) {
        posBank = staticParam.posBank;
    } else {
        size_t rgbNum = sizeof(typeRGBParam) / sizeof(string);
        staticParam.configRGB.reserve(getBankNum() * rgbNum);
        staticParam.configRGBIndex.resize(getBankNum());
        posBank = bank;
    }

    /** Init Color Martrix */
    if (!readColorConfigFromFile((workPath + "COLORMATRIX.txt").c_str())) {
        mDebug(1, "Can't open color matrix.txt");
    }

    switchBank(posBank);

    inited = true;
}

int pParam::searchFileCnt(const char* path, const char* regex) const
{
    FATFS_DIR dir;
    FRESULT fres;
    FILINFO fno;
    int cnt = 0;

    fres = f_opendir(&dir, path);
    if (fres != FR_OK) {
        mDebug(DEBUG_LEVEL_ERROR, "Can't open dir:%s:%d", path, int(fres));
        return 0;
    }

    while ((fres = f_readdir(&dir, &fno)) == FR_OK && fno.fname[0] != '\0') {
        if (re_match(regex, fno.fname) != -1) {
            cnt += 1;
        }
    }

    f_closedir(&dir);
    return cnt;
}

bool pParam::searchFileName(const char* path,
                            const char* regex,
                            std::string& out,
                            int pos) const
{
    FATFS_DIR dir;
    FRESULT fres;
    FILINFO fno;
    int cnt = 0;
    bool matched = false;

    fres = f_opendir(&dir, path);
    if (fres != FR_OK) {
        mDebug(DEBUG_LEVEL_ERROR, "Can't open dir:%s:%d", path, int(fres));
        return 0;
    }

    while ((fres = f_readdir(&dir, &fno)) == FR_OK && fno.fname[0] != '\0') {
        if (re_match(regex, fno.fname) != -1) {
            cnt += 1;
        } else {
            continue;
        }
        if (cnt - 1 == pos) {
            matched = true;
            out = fno.fname;
            break;
        }
    }

    f_closedir(&dir);
    return cnt;
}

bool pParam::readColorConfigFromFile(const char *filepath)
{
    FIL file;
    FRESULT fres;
    const int lineBufferLen = 256;
    char *lineBuffer = (char*)malloc(sizeof(TCHAR)*lineBufferLen);
    fres = f_open(&file, filepath, FA_READ);
    if (fres != FR_OK) {
        free(lineBuffer);
        return false;
    }
    while (f_gets((TCHAR*)lineBuffer, lineBufferLen, &file))
    {
        setColorParameterFromLine(lineBuffer);
    }
    f_close(&file);
    free(lineBuffer);
    return true;
}

bool pParam::readConfigFromFile(const char *filepath)
{
    FIL file;
    FRESULT fres;
    const int lineBufferLen = 256;
    char *lineBuffer = (char*)malloc(sizeof(TCHAR)*lineBufferLen);
    fres = f_open(&file, filepath, FA_READ);
    if (fres != FR_OK) {
        free(lineBuffer);
        return false;
    }
    while (f_gets((TCHAR*)lineBuffer, lineBufferLen, &file))
    {
        setParameterFromLine(lineBuffer);
    }
    f_close(&file);
    free(lineBuffer);
    return true;
}

bool pParam::readStaticParameter()
{
    return false;
}

bool pParam::writeStaticParameter()
{
    return false;
}
