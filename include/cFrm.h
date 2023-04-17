#ifndef __C_FRM_H
#define __C_FRM_H

#include "utils.h"
#include "cError.h"
#include "cFrmFrame.h"
#include <vector>

#include <cstring>

namespace falloutLib
{
    typedef struct pixelShiftDirectionStruct {
        short orientation[6];
    } pixelShiftDirectionStruct;

    class cFrm: public cError
    {
    public:
        cFrm();
        cFrm(const char *filename);
        ~cFrm();
        errorResult loadFrm(const char *filename);
        cFrmFrame * getFrame(unsigned int direction = 0, unsigned int frameNum = 0);

    private:
        unsigned int version;
        unsigned short fps;
        unsigned short noOfFramesPerDirection;
        unsigned short actionFrame;
        pixelShiftDirectionStruct pixelShiftDirection[2];
        unsigned long offsetForFrameArea[6];
        unsigned long sizeOfFrameData;
        unsigned int orientationsStored;

        std::vector<cFrmFrame*> frmFrames[6];
    };
}

#endif