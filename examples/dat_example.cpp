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


    sFILE *filetest = datFopen("art\\critters\\nmwarrrg.frm"); // text\\english\\game\\stat.msg");

    return 0;
}