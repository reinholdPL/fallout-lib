#include "utils.h"

namespace falloutLib
{
    char *loadFileContent(const char *filename)
    {
        char *buffer = nullptr;
        FILE *input = fopen(filename, "rb");
        if (input == nullptr)
        {
            return nullptr;
        }

        fseek(input, 0, SEEK_END);
        unsigned long fileSize = ftell(input);
        buffer = new char[fileSize];
        fseek(input, 0, SEEK_SET);

        fread(buffer, fileSize, 1, input);
        fclose(input);

        return buffer;
    }

    unsigned char readByteFromBuffer(const char *buffer, unsigned long offset)
    {
        return buffer[offset];
    }

    unsigned short readUInt16BEFromBuffer(const char *buffer, unsigned long offset)
    {
        unsigned short result;
        char *ptr = (char *)&result;
        ptr[1] = buffer[offset];
        ptr[0] = buffer[offset+1];
        return result;
    }

    unsigned short readUInt32BEFromBuffer(const char *buffer, unsigned long offset)
    {
        unsigned int result = 32;
        char *ptr = (char *)&result;
        ptr[3] = buffer[offset ];
        ptr[2] = buffer[offset +1];
        ptr[1] = buffer[offset +2];
        ptr[0] = buffer[offset +3];

        return result;
    }

    void dumpColor(cPalStr col)
    {
        printf("R:%u G:%u B:%u\n", col.r, col.g, col.b);
    }

    const char* errCodeToString(errorResult err) {
        return stringErrorValues[err];
    }
}