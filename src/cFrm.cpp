#include "cFrm.h"

namespace falloutLib
{
    cFrm::cFrm()
    {
    }

    cFrm::cFrm(const char *filename) : cFrm()
    {
        loadFrm(filename);
    }

    errorResult cFrm::loadFrm(const char *filename)
    {
        printf("loading: %s\n",filename);


        return NO_ERROR;
    }

}