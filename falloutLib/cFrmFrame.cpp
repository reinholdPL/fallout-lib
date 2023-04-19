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

    char *cFrmFrame::render(cPalette palette, bool transparency)
    {
        char *result;

        if (!pixelData)
            return nullptr;

        unsigned int bytesPerPixel = transparency?4:3;
        result = new char[width*height*bytesPerPixel]; //memory for 24bpp image
        for (unsigned int y=0; y<height; y++) {
            for (unsigned int x=0; x<width; x++) {
                unsigned int colorIdx = pixelData[y*width+x];
                cPalStr color = palette.getColorByIndex(colorIdx);
                
                result[(y*width+x)*bytesPerPixel] = color.r;
                result[(y*width+x)*bytesPerPixel+1] = color.g;
                result[(y*width+x)*bytesPerPixel+2] = color.b;

                if (bytesPerPixel == 4) {
                    result[(y*width+x)*bytesPerPixel+3] = colorIdx==0?0:255;
                }
            }
        }
        
        return result;
    }
}