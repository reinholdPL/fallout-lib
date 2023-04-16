#include <cstdio>
#include "falloutLib.h"

int main()
{
    falloutLib::errorResult error = falloutLib::NO_ERROR;
    falloutLib::cPalette pal;

    error = pal.loadPalette("/home/rein/Syncthing/projekty/frm2img/color.pal");

    if (error)
    {
        return 1;
    }

    // /home/rein/Syncthing/projekty/fallout/falldemo/art/intrface/skildex.frm
    // for (int i=2; i<10; i++) {
    //     falloutLib::dumpColor( pal.getColorByIndex(i) );
    // }

    printf("DONE.\n");
    return 0;
}