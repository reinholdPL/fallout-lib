#include "cFrmFrame.h"

cFrmFrame::cFrmFrame()
{
    width = 0;
    height = 0;
    pixelData = nullptr;
}

cFrmFrame::~cFrmFrame()
{
    if (pixelData != nullptr)
    {
        delete pixelData;
        pixelData=nullptr;
    }
}

cFrmFrame::cFrmFrame(unsigned int w, unsigned int h) : cFrmFrame()
{
    width = w;
    height = h;
}

void cFrmFrame::setPixelData(char *data)
{
    pixelData = data;

    // delete  pixelData;
    //pixelData = nullptr;
}

void cFrmFrame::setWidth(unsigned int w)
{
    width = w;
}

void cFrmFrame::setHeight(unsigned int h)
{
    height = h;
}

void cFrmFrame::setPixelShift(unsigned int direction, short val)
{
    pixelShift[direction] = val;
}