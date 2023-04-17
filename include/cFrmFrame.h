#ifndef __C_FRM_FRAME_H
#define __C_FRM_FRAME_H

#include <cstdio>

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

private:
    char *pixelData;
    unsigned int width;
    unsigned int height;
    short pixelShift[2];
};

#endif