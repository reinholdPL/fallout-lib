#ifndef __C_FRM_H
#define __C_FRM_H

#include "utils.h"
#include "cError.h"

namespace falloutLib
{
    class cFrm: public cError
    {
    public:
        cFrm();
        cFrm(const char *filename);
        errorResult loadFrm(const char *filename);

    private:
        unsigned int version;
        unsigned short fps;
        unsigned short noOfFramesPerDirection;
        unsigned short actionFrame;
    };

}

#endif