#include <cList.h>
#include <cDAT.h>
#include <algorithm>

namespace falloutLib
{

    static inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    cList::cList()
    {
    }

    cList::cList(const char *filename)
    {
        loadList(filename);
    }

    errorResult cList::loadList(const char *filename)
    {
        setErrorState(NO_ERROR);

        char *listData = nullptr;
        unsigned long currentFileOffset = 0;
        sFILE *listFile = datFopen(filename);

        if (listFile == nullptr)
        {
            setErrorState(NO_FILE);
            return NO_FILE;
        }

        listData = (char *)listFile->buffer;

        char *split_request;
        split_request = strtok(listData, "\r\n");
        while (split_request != NULL)
        {
            std::string res = split_request;
            rtrim(res);


            entries.push_back(res.c_str());
            split_request = strtok(NULL, "\r\n");
        }

        // for (int i=1; i<=entries.size(); i++) {
        //     printf("%d = '%s'\n", i, getEntryByNum(i));
        // }

        free(listData);
        return getErrorState();
    }

    const char *cList::getEntryByNum(unsigned short num)
    {
        if (num - 1 < entries.size())
            return entries[num].c_str();
        else
            return nullptr;
    }

}