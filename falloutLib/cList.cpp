#include <cList.h>

namespace falloutLib
{
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
        listData = loadFileContent(filename);

        if (listData == nullptr)
        {
            setErrorState(NO_FILE);
            return NO_FILE;
        }

        char *split_request;
        split_request = strtok(listData, "\r\n");
        while (split_request != NULL)
        {
            entries.push_back(split_request);
            split_request = strtok(NULL, "\r\n");
        }

        // for (int i=1; i<=entries.size(); i++) {
        //     printf("%d = '%s'\n", i, getEntryByNum(i));
        // }

        free(listData);
        return getErrorState();
    }

    const char* cList::getEntryByNum(unsigned short num) {
        if (num-1 < entries.size())
            return entries[num].c_str();
        else return nullptr;
    }

}