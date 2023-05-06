#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <falloutLib.h>
#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include <png.h>
#include <regex>

falloutLib::cPalette pal;
using namespace cv;
int scroll_x = 1140;
int scroll_y = -1340;

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
    unsigned int tilesize = 256;
    const Size outputSize = Size(tilesize * 3, tilesize * 3);
    Point2f targetVertices[4] = {
        Point(0, 0),
        Point(outputSize.width - 1, 0),
        Point(outputSize.width - 1, outputSize.height - 1),
        Point(0, outputSize.height - 1),
    };

    Mat rotationMatrix = getPerspectiveTransform(vertices, targetVertices);
    warpPerspective(frame, result, rotationMatrix, outputSize, flags);
}

const char *paletteFilename = "../assets/color.pal";
const char *frmFilename = "/home/rein/Syncthing/projekty/fallout/falldemo/art/tiles/grid001.frm";
const char *mapFilename = "maps\\kladwtwn.map";
const char *tilesLstFilename = "art\\tiles\\tiles.lst";
std::string tilesPath = "art\\tiles\\";
SDL_Surface *tilesSurface, *oneTileSurface;
std::map<unsigned int, SDL_Surface *> tiles;

void draw_map(falloutLib::cMap map, falloutLib::cList list)
{
    // GENERATING SURFACE WITH TILES

    SDL_FillRect(tilesSurface, NULL, 0x000000);

    float pos_x = (float)scroll_x; // 48 + 48 + 5;
    float pos_y = (float)scroll_y; // 0 + 5;

    // printf("%f %f\n", pos_x, pos_y);

    float horizontal_spacing = 48;
    float vertical_spacing = 12;
    float percent_add = 0; // 0.15;

    if (percent_add > 0)
    {
        horizontal_spacing += horizontal_spacing * percent_add;
        vertical_spacing += vertical_spacing * percent_add;
    }

    int side_length = 100;
    int tile_offset_x = 0;
    int tile_offset_y = 0;

    for (int y = 0; y < side_length; y++)
    {
        for (int x = 0; x < side_length; x++)
        {
            int tileToDraw = map.getElevation(0).getRoofTile(tile_offset_x + x, tile_offset_y + y);
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
}

void generate_one_tile(int map[3][3], falloutLib::cList list)
{
    // GENERATING SURFACE WITH TILES

    unsigned int tilesize = 256;

    std::string outputFilename = "";

    SDL_FillRect(oneTileSurface, NULL, 0x000000);

    float pos_x = 48 + 48 + 5;
    float pos_y = 0 + 5;

    // printf("%f %f\n", pos_x, pos_y);

    float horizontal_spacing = 48;
    float vertical_spacing = 12;
    float percent_add = 0; // 0.15;

    if (percent_add > 0)
    {
        horizontal_spacing += horizontal_spacing * percent_add;
        vertical_spacing += vertical_spacing * percent_add;
    }

    int side_length = 3;
    int tile_offset_x = 0;
    int tile_offset_y = 0;

    for (int y = 0; y < side_length; y++)
    {
        for (int x = 0; x < side_length; x++)
        {
            int tileToDraw = map[x][y];

            if (tiles.count(tileToDraw) == 0)
            {
                tiles[tileToDraw] = createSurfaceFromFrm((tilesPath + list.getEntryByNum(tileToDraw)).c_str());
            }

            if (x == 1 && y == 1)
            {
                outputFilename = list.getEntryByNum(tileToDraw);
            }

            if (tileToDraw != 1)
            {
                blitSurface(tiles[tileToDraw], oneTileSurface, pos_x, pos_y);
            }
            pos_x -= horizontal_spacing;
            pos_y += vertical_spacing;
        }

        pos_x += side_length * horizontal_spacing;
        pos_y -= vertical_spacing * side_length;

        pos_x += horizontal_spacing * (2.0 / 3.0); //(2/3)* horizontal_spacing;
        pos_y += 2 * vertical_spacing;
    }

    cv::Mat image;
    image = Mat(400, 400, CV_8UC4, oneTileSurface->pixels);

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
    // fourPointsTransform(image, &quadrangle_2f[0], wynik2, INTER_AREA);
    // fourPointsTransform(image, &quadrangle_2f[0], wynik3, INTER_CUBIC);
    //fourPointsTransform(image, &quadrangle_2f[0], wynik, INTER_LANCZOS4);

    Mat cropped;
    wynik(Rect(tilesize, tilesize, tilesize, tilesize)).copyTo(cropped);

    png_image pngimage_control;
    memset(&pngimage_control, 0, sizeof(png_image));

    pngimage_control.version = PNG_IMAGE_VERSION;
    pngimage_control.width = tilesize;
    pngimage_control.height = tilesize;
    pngimage_control.format = PNG_FORMAT_BGRA;
    // pngimage_control.flags = PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB;

    outputFilename = std::regex_replace(outputFilename, std::regex(".frm"), "");
    std::string output_filename = "../assets/art/tiles_cache/" + outputFilename + ".png";
    printf("writing to = %s\n", output_filename.c_str());
    int success = png_image_write_to_file(&pngimage_control,       // control str
                                          output_filename.c_str(), // file path
                                          0,                       // convert_to_8bit
                                          cropped.data,            // buffer
                                          0,                       // row_stride
                                          NULL);                   // colormap
    if (!success)
    {
        fprintf(stderr, "Writing png file failed to path: .\n");
    }
    else
    {
         fprintf(stderr, "PNG file saved: %s\n", output_filename.c_str());
    }

    //  imshow("image", image);
    // moveWindow("image", 0, 0);
    // imshow("wynik", wynik);
    //  moveWindow("wynik", 128 * 3, 0);
    // imshow("wynik2", wynik2);
    // moveWindow("wynik2", 128 * 3 * 2, 0);
    // imshow("wynik3", wynik3);
    // moveWindow("wynik3", 128 * 3 * 3, 0);
    // imshow("wynik4", wynik4);
    // moveWindow("wynik4", 128 * 3 * 4, 0);

    // imshow("cropped", cropped);
    // moveWindow("cropped", 0, 200);

    // int k = waitKey(0); // Wait for a keystroke in the window

    // destroyWindow("wynik");
}

std::vector<std::string> convertedTiles;

template <typename T>
const bool contains(std::vector<T> &Vec, const T &Element)
{
    if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
        return true;

    return false;
}

void generate_converted_tiles(falloutLib::cMap map, falloutLib::cList list)
{

    int side_length = 100;
    int elevation = 0;

    int tempMap[3][3];

    for (int y = 0; y < side_length; y++)
    {
        for (int x = 0; x < side_length; x++)
        {
            int tileToDraw = map.getElevation(elevation).getRoofTile(x, y);
            if (tileToDraw == 1)
                continue;

            unsigned int num_tiles[9] = {
                map.getElevation(elevation).getRoofTile(x - 1, y - 1),
                map.getElevation(elevation).getRoofTile(x, y - 1),
                map.getElevation(elevation).getRoofTile(x + 1, y - 1),
                map.getElevation(elevation).getRoofTile(x - 1, y),
                map.getElevation(elevation).getRoofTile(x, y),
                map.getElevation(elevation).getRoofTile(x + 1, y),
                map.getElevation(elevation).getRoofTile(x - 1, y + 1),
                map.getElevation(elevation).getRoofTile(x, y + 1),
                map.getElevation(elevation).getRoofTile(x + 1, y + 1)};

            // bool skip = false;
            // for (unsigned int i=0; i<9; i++) {
            //     if (num_tiles[i] == 1) skip = true;
            // }
            // if (skip) continue;

            std::string tileName = "";
            // for (unsigned int t = 0; t < 8; t++)
            // {
            //     tileName += std::to_string(num_tiles[t]) + "_";
            // }
            // tileName += std::to_string(num_tiles[8]);

            tileName = std::to_string(num_tiles[4]);

            // printf("%s\n", tileName.c_str());

            if (!contains(convertedTiles, tileName))
            {
                convertedTiles.push_back(tileName);

                tempMap[0][0] = num_tiles[0];
                tempMap[1][0] = num_tiles[1];
                tempMap[2][0] = num_tiles[2];
                tempMap[0][1] = num_tiles[3];
                tempMap[1][1] = num_tiles[4];
                tempMap[2][1] = num_tiles[5];
                tempMap[0][2] = num_tiles[6];
                tempMap[1][2] = num_tiles[7];
                tempMap[2][2] = num_tiles[8];

                // printf("x=%d y=%d\n", x, y);
                generate_one_tile(tempMap, list);

                // return;

                // if (x==100 ) return;
            }
        }
    }

    printf("convertedTiles len = %d\n", convertedTiles.size());
    for (int i = 0; i < convertedTiles.size(); i++)
    {
        //printf("i: %d %s\n", i, convertedTiles[i].c_str());
    }
}

int main(int argc, char **argv)
{
    falloutLib::registerDATfile("../assets/master.dat");
    falloutLib::registerDATfile("../assets/critter.dat");

    // image = cv::imread( argv[1], cv::IMREAD_COLOR );

    SDL_Window *win = nullptr;
    SDL_Surface *windowSurface = nullptr;
    SDL_Surface *frmImage = nullptr;

    bool closeWindow = false;

    falloutLib::cList list(tilesLstFilename);
    falloutLib::cMap map = falloutLib::cMap(mapFilename);

    if (map.getErrorState())
    {
        printf("%s: %s\n", mapFilename, falloutLib::errCodeToString(map.getErrorState()));
        return 1;
    }

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
                           1024, 1024, 0);

    windowSurface = SDL_GetWindowSurface(win);
    frmImage = createSurfaceFromFrm(frmFilename);

    tilesSurface = SDL_CreateRGBSurface(0, 1400, 1400, 32, 0, 0, 0, 0);
    oneTileSurface = SDL_CreateRGBSurface(0, 400, 400, 32, 0, 0, 0, 0);

    // image = Mat(400, 400, CV_8UC4, tilesSurface->pixels);

    // // image = cv2.cvtColor(image, cv2.CV_BGRA2BGR) //DROP ALPHA

    // Mat wynik, wynik2, wynik3, wynik4;

    // std::vector<Point2f> quadrangle_2f;

    // quadrangle_2f.emplace_back(Point(198 - 50, 74 - 70));
    // quadrangle_2f.emplace_back(Point(294 - 50, 146 - 70));
    // quadrangle_2f.emplace_back(Point(150 - 50, 183 - 70));
    // quadrangle_2f.emplace_back(Point(54 - 50, 110 - 70));

    // // for (int i = 0; i < quadrangle_2f.size(); i++)
    // //     cv::circle(image, quadrangle_2f[i], 2, cv::Vec3f(255, 255, 255), -1);

    // fourPointsTransform(image, &quadrangle_2f[0], wynik);
    // fourPointsTransform(image, &quadrangle_2f[0], wynik2, INTER_AREA);
    // fourPointsTransform(image, &quadrangle_2f[0], wynik3, INTER_CUBIC);
    // fourPointsTransform(image, &quadrangle_2f[0], wynik4, INTER_LANCZOS4);

    // Mat cropped;
    // wynik4(Rect(128, 128, 128, 128)).copyTo(cropped);

    // // imshow("image", image);
    // // moveWindow("image", 0, 0);
    // // imshow("wynik", wynik);
    // // moveWindow("wynik", 128 * 3, 0);
    // // imshow("wynik2", wynik2);
    // // moveWindow("wynik2", 128 * 3 * 2, 0);
    // // imshow("wynik3", wynik3);
    // // moveWindow("wynik3", 128 * 3 * 3, 0);
    // // imshow("wynik4", wynik4);
    // // moveWindow("wynik4", 128 * 3 * 4, 0);

    // // imshow("cropped", cropped);
    // // moveWindow("cropped", 128 * 3 * 5, 0);

    // // int k = waitKey(0); // Wait for a keystroke in the window

    generate_converted_tiles(map, list);

    while (!closeWindow)
    {
        SDL_FillRect(windowSurface, NULL, 0x000000);

        draw_map(map, list);
        // blitSurface(oneTileSurface, windowSurface, 0, 0);

        blitSurface(tilesSurface, windowSurface, 0, 0);
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            {
                closeWindow = 1;
            }

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    scroll_y += 20;
                    break;
                case SDLK_s:
                    scroll_y -= 20;
                    break;
                case SDLK_a:
                    scroll_x += 20;
                    break;
                case SDLK_d:
                    scroll_x -= 20;
                    break;
                default:
                    break;
                }
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