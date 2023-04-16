#ifndef __C_PALETTE_H
#define __C_PALETTE_H

#include "utils.h"

namespace falloutLib
{
    class cPalette
    {
    public:
        cPalette();
        errorResult loadPalette(const char *filename);
        cPalStr getColorByIndex(unsigned char idx);

    private:
        cPalStr colors[256];
    };

}

#endif