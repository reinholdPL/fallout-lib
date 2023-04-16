#ifndef __UTILS_H
#define __UTILS_H

#include <cstdio>
#include <cstdlib>

namespace falloutLib
{
    static const char *const stringErrorValues[] = {
        "No error",
        "File not found"};

    typedef enum
    {
        NO_ERROR,
        NO_FILE,
        FRM_VERSION_NOT_SUPPORTED
    } errorResult;

    typedef struct cPalStr
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    } cPalStr;

    char *loadFileContent(const char *filename);
    unsigned char readByteFromBuffer(const char *buffer, unsigned long offset);
    unsigned short readUInt16BEFromBuffer(const char *buffer, unsigned long offset);
    unsigned short readUInt32BEFromBuffer(const char *buffer, unsigned long offset);
    void dumpColor(cPalStr col);
    const char* errCodeToString(errorResult err);

}

#endif