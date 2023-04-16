#include <cstdio>
#include "falloutLib.h"

int main()
{
    const char *paletteFilename = "/home/rein/Syncthing/projekty/frm2img/color.pal";
    const char *frmFilename = "/home/rein/Syncthing/projekty/fallout/falldemo/art/intrface/skildex.frm";

    falloutLib::cPalette pal(paletteFilename);
    falloutLib::cFrm frm;

    if (pal.getErrorState())
    {
        printf("%s: %s\n", paletteFilename, falloutLib::errCodeToString( pal.getErrorState() ));
        return 1;
    }

    frm.loadFrm(frmFilename);
    if (frm.getErrorState())
    {
        printf("%s: %s\n", frmFilename, falloutLib::errCodeToString( frm.getErrorState() ));
        return 1;
    }
    // 
    // for (int i=2; i<10; i++) {
    //     falloutLib::dumpColor( pal.getColorByIndex(i) );
    // }

    printf("DONE.\n");
    return 0;
}