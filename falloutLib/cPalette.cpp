#include "cPalette.h"

namespace falloutLib
{
    cPalette::cPalette()
    {
    }

    cPalette::cPalette(const char *filename)
    {
        loadPalette(filename);
    }

    errorResult cPalette::loadPalette(const char *filename)
    {
        setErrorState(NO_ERROR);

        char *palData = nullptr;
        unsigned long currentFileOffset = 0;
        palData = loadFileContent(filename);

        if (palData == nullptr)
        {
            setErrorState(NO_FILE);
            return NO_FILE;
        }

        for (unsigned int i = 0; i < 256; i++)
        {
            colors[i] = {
                .r = (unsigned char)(4 * readByteFromBuffer(palData, currentFileOffset)),
                .g = (unsigned char)(4 * readByteFromBuffer(palData, currentFileOffset + 1)),
                .b = (unsigned char)(4 * readByteFromBuffer(palData, currentFileOffset + 2)),
            };

            currentFileOffset += 3;
        }

        free(palData);

        return NO_ERROR;
    }

    cPalStr cPalette::getColorByIndex(unsigned char idx)
    {
        return colors[idx];
    }

}