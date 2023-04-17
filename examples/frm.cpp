#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include "falloutLib.h"

int main()
{
    SDL_Window *win = nullptr;
    SDL_Surface *windowSurface = nullptr;
    SDL_Surface *frmImage = nullptr;
    bool closeWindow = false;
    const char *paletteFilename = "../assets/color.pal";
    const char *frmFilename = "../assets/skildex.frm";

    falloutLib::cPalette pal(paletteFilename);
    falloutLib::cFrm frm;
    falloutLib::cFrmFrame *frame = nullptr;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (pal.getErrorState())
    {
        printf("%s: %s\n", paletteFilename, falloutLib::errCodeToString(pal.getErrorState()));
        return 1;
    }

    frm.loadFrm(frmFilename);
    if (frm.getErrorState())
    {
        printf("%s: %s\n", frmFilename, falloutLib::errCodeToString(frm.getErrorState()));
        return 1;
    }

    frame = frm.getFrame(0, 0);

    win = SDL_CreateWindow("frm example",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       frame->getWidth(), frame->getHeight(), 0);

    char *pixels = frame->render(pal);

    windowSurface = SDL_GetWindowSurface(win);
    frmImage =
        SDL_CreateRGBSurfaceFrom(pixels, frame->getWidth(), frame->getHeight(),
                                 24, //
                                 frame->getWidth() * 3,
                                 0xFF,
                                 0xFF << 8,
                                 0xFF << 16,
                                 0);

    SDL_BlitSurface(frmImage, NULL, windowSurface, NULL);
    while (!closeWindow)
    {
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

    delete pixels;

    return 0;
}