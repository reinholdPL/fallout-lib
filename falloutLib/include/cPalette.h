#ifndef __C_PALETTE_H
#define __C_PALETTE_H

#include "utils.h"
#include "cError.h"

namespace falloutLib
{
    class cPalette: public cError
    {
    public:
        cPalette();
        cPalette(const char *filename);
        errorResult loadPalette(const char *filename);
        cPalStr getColorByIndex(unsigned char idx);

    private:
        cPalStr colors[256];
        
    };
}

#endif