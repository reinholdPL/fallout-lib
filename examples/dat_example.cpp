#include <falloutLib.h>

using namespace falloutLib;

int main(int argc, char **argv)
{
    registerDATfile("../assets/master.dat");
    registerDATfile("../assets/critter.dat");

    //unsigned char *testfile = masterDAT.getFileByName("text\\english\\game\\stat.msg");
    //printf("%s\n", testfile);
    //delete testfile;
    // printf("%d\n", datFiles.size());


    sFILE *filetest = datFopen("text\\english\\game\\stat.msg"); // text\\english\\game\\stat.msg");
    if (filetest) {
        printf("GOT FILE! %d\n", 32);
    }

    printf("%s\n", filetest->buffer);

    return 0;
}