#ifndef __C_FRM_FRAME_H
#define __C_FRM_FRAME_H

#include "cPalette.h"
#include <cstdio>

namespace falloutLib
{
    class cFrmFrame
    {
    public:
        cFrmFrame();
        cFrmFrame(unsigned int w, unsigned int h);
        ~cFrmFrame();
        void setWidth(unsigned int w);
        void setHeight(unsigned int h);
        void setPixelShift(unsigned int direction, short val);
        void setPixelData(char *data);

        unsigned int getWidth();
        unsigned int getHeight();
        short getPixelShift(unsigned int direction);
        char *getPixelData(char *data);
        char *render(cPalette palette);

    private:
        char *pixelData;
        unsigned int width;
        unsigned int height;
        short pixelShift[2];
    };
}

#endif