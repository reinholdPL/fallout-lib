#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <falloutLib.h>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>

falloutLib::cPalette pal;
using namespace cv;

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

    memcpy(frmImage->pixels, pixels, frame->getWidth() * frame->getHeight() * 4);
    delete pixels;

    return frmImage;
}

void blitSurface(SDL_Surface *src, SDL_Surface *dst, int x, int y)
{
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;

    SDL_BlitSurface(src, NULL, dst, &dstRect);
}

void fourPointsTransform(const Mat &frame, const Point2f vertices[], Mat &result, InterpolationFlags flags = INTER_NEAREST)
{
    const Size outputSize = Size(128 * 3, 128 * 3);
    Point2f targetVertices[4] = {
        Point(0, 0),
        Point(outputSize.width - 1, 0),
        Point(outputSize.width - 1, outputSize.height - 1),
        Point(0, outputSize.height - 1),
    };

    Mat rotationMatrix = getPerspectiveTransform(vertices, targetVertices);
    warpPerspective(frame, result, rotationMatrix, outputSize, flags);
}

int main(int argc, char **argv)
{
    cv::Mat image;

    // image = cv::imread( argv[1], cv::IMREAD_COLOR );

    SDL_Window *win = nullptr;
    SDL_Surface *windowSurface = nullptr;
    SDL_Surface *frmImage = nullptr;

    std::map<unsigned int, SDL_Surface *> tiles;
    bool closeWindow = false;
    const char *paletteFilename = "../assets/color.pal";
    const char *frmFilename = "/home/rein/Syncthing/projekty/fallout/falldemo/art/tiles/grid001.frm";
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

    std::string tilesPath = "/home/rein/Syncthing/projekty/fallout/falldemo/art/tiles/";

    SDL_Surface *tilesSurface = SDL_CreateRGBSurface(0, 400, 400, 32, 0, 0, 0, 0);

    // GENERATING SURFACE WITH TILES

    float pos_x = 48 + 48 + 5;
    float pos_y = 0 + 5;

    float horizontal_spacing = 48;
    float vertical_spacing = 12;
    float percent_add = 0; // 0.15;

    if (percent_add > 0)
    {
        horizontal_spacing += horizontal_spacing * percent_add;
        vertical_spacing += vertical_spacing * percent_add;
    }

    int side_length = 3;
    int tile_offset_x = 48;
    int tile_offset_y = 49;

    for (int y = 0; y < side_length; y++)
    {
        for (int x = 0; x < side_length; x++)
        {
            int tileToDraw = map.getElevation(0).getFloorTile(tile_offset_x + x, tile_offset_y + y);
            // printf("tileToDraw: %d\n", tileToDraw);
            if (tiles.count(tileToDraw) == 0)
            {
                tiles[tileToDraw] = createSurfaceFromFrm((tilesPath + list.getEntryByNum(tileToDraw)).c_str());
            }
            if (tileToDraw != 1)
            {
                blitSurface(tiles[tileToDraw], tilesSurface, pos_x, pos_y);
            }
            pos_x -= horizontal_spacing;
            pos_y += vertical_spacing;
        }

        pos_x += side_length * horizontal_spacing;
        pos_y -= vertical_spacing * side_length;

        pos_x += horizontal_spacing * (2.0 / 3.0); //(2/3)* horizontal_spacing;
        pos_y += 2 * vertical_spacing;
    }

    image = Mat(400, 400, CV_8UC4, tilesSurface->pixels);

    // image = cv2.cvtColor(image, cv2.CV_BGRA2BGR) //DROP ALPHA

    Mat wynik, wynik2, wynik3, wynik4;

    std::vector<Point2f> quadrangle_2f;

    quadrangle_2f.emplace_back(Point(198 - 50, 74 - 70));
    quadrangle_2f.emplace_back(Point(294 - 50, 146 - 70));
    quadrangle_2f.emplace_back(Point(150 - 50, 183 - 70));
    quadrangle_2f.emplace_back(Point(54 - 50, 110 - 70));

    // for (int i = 0; i < quadrangle_2f.size(); i++)
    //     cv::circle(image, quadrangle_2f[i], 2, cv::Vec3f(255, 255, 255), -1);

    fourPointsTransform(image, &quadrangle_2f[0], wynik);
    fourPointsTransform(image, &quadrangle_2f[0], wynik2, INTER_AREA);
    fourPointsTransform(image, &quadrangle_2f[0], wynik3, INTER_CUBIC);
    fourPointsTransform(image, &quadrangle_2f[0], wynik4, INTER_LANCZOS4);

    Mat cropped;
    wynik4(Rect(128, 128, 128, 128)).copyTo(cropped);

    imshow("image", image);
    moveWindow("image", 0, 0);
    imshow("wynik", wynik);
    moveWindow("wynik", 128 * 3, 0);
    imshow("wynik2", wynik2);
    moveWindow("wynik2", 128 * 3 * 2, 0);
    imshow("wynik3", wynik3);
    moveWindow("wynik3", 128 * 3 * 3, 0);
    imshow("wynik4", wynik4);
    moveWindow("wynik4", 128 * 3 * 4, 0);

    imshow("cropped", cropped);
    moveWindow("cropped", 128 * 3 * 5, 0);

    int k = waitKey(0); // Wait for a keystroke in the window

    while (!closeWindow)
    {
        SDL_FillRect(windowSurface, NULL, 0x000000);
        blitSurface(tilesSurface, windowSurface, 0, 0);
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
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