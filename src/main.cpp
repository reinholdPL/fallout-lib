#include <cstdio>
#include "falloutLib.h"

int main()
{
    const char *paletteFilename = "/home/rein/Synctehing/projekty/frm2img/color.pal";
    falloutLib::cPalette pal(paletteFilename);

    if (pal.getErrorState())
    {
        printf("%s: %s\n", paletteFilename, falloutLib::errCodeToString( pal.getErrorState() ));
        return 1;
    }

    // /home/rein/Syncthing/projekty/fallout/falldemo/art/intrface/skildex.frm
    // for (int i=2; i<10; i++) {
    //     falloutLib::dumpColor( pal.getColorByIndex(i) );
    // }

    printf("DONE.\n");
    return 0;
}