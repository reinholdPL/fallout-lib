#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <falloutLib.h>

falloutLib::cPalette pal;

SDL_Surface *createSurfaceFromFrm(const char *frmFilename)
{
    falloutLib::cFrm frm;
    falloutLib::cFrmFrame *frame = nullptr;

    frm.loadFrm(frmFilename);
    if (frm.getErrorState())
    {
        printf("%s: %s\n", frmFilename, falloutLib::errCodeToString(frm.getErrorState()));
        return nullptr;
    }

    frame = frm.getFrame(0, 0);

    SDL_Surface *frmImage =
        SDL_CreateRGBSurface(0, frame->getWidth(), frame->getHeight(),
                             32,
                             0xFF,
                             0xFF << 8,
                             0xFF << 16,
                             0xFF << 24);

    char *pixels = frame->render(pal, true);

    memcpy(frmImage->pixels, pixels, frame->getWidth()*frame->getHeight()*4);
    delete pixels;

    return frmImage;
}

// SDL_Surface* SDL_CreateRGBSurface
//     (Uint32 flags, int width, int height, int depth,
//      Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

int main()
{
    SDL_Window *win = nullptr;
    SDL_Surface *windowSurface = nullptr;
    SDL_Surface *frmImage = nullptr;
    bool closeWindow = false;
    const char *paletteFilename = "../assets/color.pal";
    const char *frmFilename = "/home/rein/Syncthing/projekty/fallout/falldemo/art/tiles/bld1050.frm";
    const char *mapFilename = "/home/rein/Syncthing/projekty/fallout/falldemo/maps/city1.map";
    const char *tilesLstFilename = "/home/rein/Syncthing/projekty/fallout/falldemo/art/tiles/tiles.lst";

    falloutLib::cList list(tilesLstFilename);
    falloutLib::cMap map = falloutLib::cMap(mapFilename);

    pal.loadPalette(paletteFilename);

    if (pal.getErrorState())
    {
        printf("%s: %s\n", paletteFilename, falloutLib::errCodeToString(pal.getErrorState()));
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    win = SDL_CreateWindow("frm exampleA",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           1024, 768, 0);

    windowSurface = SDL_GetWindowSurface(win);
    frmImage = createSurfaceFromFrm(frmFilename);

    SDL_Rect dst;
    dst.x = 100;
    dst.y = 200;

    while (!closeWindow)
    {
        dst.x = 100;
        SDL_BlitSurface(frmImage, NULL, windowSurface, &dst);
        dst.x = 130;
        SDL_BlitSurface(frmImage, NULL, windowSurface, &dst);

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                event.type == SDL_KEYDOWN)
            {
                closeWindow = 1;
            }
        }
        SDL_UpdateWindowSurface(win);
    }

    SDL_FreeSurface(windowSurface);
    SDL_FreeSurface(frmImage);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}