#ifndef __C_DAT_H
#define __C_DAT_H

#include <utils.h>
#include <cError.h>
#include <map>
#include <vector>
#include <string>

namespace falloutLib
{
    class cDAT;
    extern std::vector<cDAT*> datFiles;
    void registerDATfile(const char *);

    typedef struct cDATFile
    {
        std::string filename;
        bool compressed;
        unsigned int realSize;
        unsigned int packedSize;
        unsigned int fileOffset;
    } cDATFile;

    typedef struct sFILE {
        unsigned char *buffer;
        unsigned long size;
        unsigned long position;
    } sFILE;

    sFILE* datFopen(const char *filename);

    class cDAT : public cError
    {
    public:
        cDAT();
        cDAT(const char *filename);
        ~cDAT();
        errorResult loadDAT(const char *filename);
        fileContent getFileByName(std::string filename);
        cDATFile getFileDataByName(std::string filename);

    private:
        FILE *input;
        std::map<std::string, cDATFile> files;
    };
}

#endif