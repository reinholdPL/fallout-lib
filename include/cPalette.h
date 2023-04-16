#ifndef __C_PALETTE_H
#define __C_PALETTE_H

#include "utils.h"

namespace falloutLib
{
    class cPalette
    {
    public:
        cPalette();
        cPalette(const char *filename);
        errorResult loadPalette(const char *filename);
        cPalStr getColorByIndex(unsigned char idx);
        errorResult getErrorState();

    private:
        cPalStr colors[256];
        errorResult errorState;
    };
}

#endif