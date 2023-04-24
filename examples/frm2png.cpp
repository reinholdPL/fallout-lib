#include <cstdio>
#include "falloutLib.h"
#include <png.h>

void write_png_file(char *filename, unsigned int width, unsigned int height, char *pixels)
{

    png_bytep *row_pointers = NULL;
    row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_bytep)pixels + (y*width*4);
    }

    int y;

    FILE *fp = fopen(filename, "wb");
    if (!fp)
        abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
        abort();

    png_infop info = png_create_info_struct(png);
    if (!info)
        abort();

    if (setjmp(png_jmpbuf(png)))
        abort();

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    // png_set_filler(png, 0, PNG_FILLER_AFTER);

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    //   for(int y = 0; y < height; y++) {
    //     free(row_pointers[y]);
    //   }
    //   free(row_pointers);

    fclose(fp);

    png_destroy_write_struct(&png, &info);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("usage: %s <input.frm> <output.png> <size>\n", argv[0]);
        return 1;
    }
    const char *paletteFilename = "/home/rein/Syncthing/git_projects/fallout-lib/assets/color.pal";
    const char *frmFilename = "../assets/skildex.frm";

    falloutLib::cPalette pal(paletteFilename);
    falloutLib::cFrm frm;
    falloutLib::cFrmFrame *frame = nullptr;

    if (pal.getErrorState())
    {
        printf("%s: %s\n", paletteFilename, falloutLib::errCodeToString(pal.getErrorState()));
        return 1;
    }

    frm.loadFrm(argv[1]);

    if (frm.getErrorState())
    {
        printf("%s: %s\n", frmFilename, falloutLib::errCodeToString(frm.getErrorState()));
        return 1;
    }

    frame = frm.getFrame(0, 0);

    char *pixels = frame->render(pal, true);

    write_png_file(argv[2], frame->getWidth(), frame->getHeight(), pixels);

    delete pixels;

    return 0;
}