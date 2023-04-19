#include "cFrmFrame.h"

namespace falloutLib
{
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
            pixelData = nullptr;
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
        // pixelData = nullptr;
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

    unsigned int cFrmFrame::getWidth()
    {
        return width;
    }
    unsigned int cFrmFrame::getHeight()
    {
        return height;
    }
    short cFrmFrame::getPixelShift(unsigned int direction)
    {
        return pixelShift[direction];
    }
    char *cFrmFrame::getPixelData(char *data)
    {
        return pixelData;
    }

    char *cFrmFrame::render(cPalette palette)
    {
        char *result;

        if (!pixelData)
            return nullptr;


        result = new char[width*height*3]; //memory for 24bpp image
        for (unsigned int y=0; y<height; y++) {
            for (unsigned int x=0; x<width; x++) {
                cPalStr color = palette.getColorByIndex(pixelData[y*width+x]);
                
                result[(y*width+x)*3] = color.r;
                result[(y*width+x)*3+1] = color.g;
                result[(y*width+x)*3+2] = color.b;
            }
        }
        
        return result;
    }
}