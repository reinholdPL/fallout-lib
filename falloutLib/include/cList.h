#ifndef __C_LIST_H
#define __C_LIST_H

#include <utils.h>
#include <cError.h>
#include <cstring>
#include <vector>
#include <string>

namespace falloutLib
{
    class cList : public cError
    {
    public:
        cList();
        cList(const char *filename);
        errorResult loadList(const char *filename);
        const char* getEntryByNum(unsigned short num);
    private:
        std::vector<std::string> entries;
    };
}

#endif