#include <cDAT.h>
#include <zlib.h>
#include <algorithm>

namespace falloutLib
{
    std::vector<cDAT *> datFiles;

    void registerDATfile(const char *filename)
    {
        cDAT *datFile = new cDAT(filename);
        datFiles.push_back(datFile);
    }

    sFILE *datFopen(const char *filename)
    {
        // art\critters\nmwarrrg.frm
        unsigned char *data = nullptr;
        for (unsigned int datFile = 0; datFile < datFiles.size(); datFile++)
        {
            data = datFiles[datFile]->getFileByName(filename);
            if (data != nullptr)
            {
                // printf("found file in dat file: %d\n", datFile);
                break;
            }
        }

        if (data == nullptr)
        { // no file in dat
            fileContent fileData = loadFileContentWithSize(filename);
        }

        return nullptr;
    }

    cDAT::cDAT()
    {
        input = nullptr;
    }

    cDAT::cDAT(const char *filename) : cDAT()
    {
        loadDAT(filename);
    }

    cDAT::~cDAT()
    {
        if (input != nullptr)
        {
            fclose(input);
            input = nullptr;
        }
    }

    errorResult cDAT::loadDAT(const char *filename)
    {
        input = fopen(filename, "rb");
        unsigned long filesize;
        unsigned int dirTreeSize;
        unsigned int filesCount;

        if (input == nullptr)
        {
            setErrorState(NO_FILE);
            return getErrorState();
        }

        fseek(input, 0, SEEK_END);
        filesize = ftell(input);
        // printf("filesize = %ld \n", filesize);

        fseek(input, filesize - 8, SEEK_SET);
        fread(&dirTreeSize, 4, 1, input);

        // printf("dirTreeSize = %d KB \n", dirTreeSize / 1024);

        fseek(input, filesize - dirTreeSize - 8, SEEK_SET);
        fread(&filesCount, 4, 1, input);

        for (unsigned int file = 0; file < filesCount; file++)
        {
            cDATFile newFile;
            unsigned int filenameLen;
            char *buffer = nullptr;
            char compressed;
            unsigned int temp;
            fread(&filenameLen, 4, 1, input);
            // printf("filename len = %d\n", filenameLen);

            buffer = new char[filenameLen + 1];
            fread(buffer, filenameLen, 1, input);
            buffer[filenameLen] = '\0';

            newFile.filename = buffer;
            delete buffer;

            fread(&compressed, 1, 1, input);
            if (compressed == 1)
                newFile.compressed = true;
            else
                newFile.compressed = false;

            fread(&temp, 4, 1, input);
            newFile.realSize = temp;
            fread(&temp, 4, 1, input);
            newFile.packedSize = temp;
            fread(&temp, 4, 1, input);
            newFile.fileOffset = temp;

            // printf("realSize = %d\n", newFile.realSize);
            // printf("packedSize = %d\n", newFile.packedSize);
            // printf("fileOffset = %d\n", newFile.fileOffset);

            std::for_each(
                newFile.filename.begin(),
                newFile.filename.end(),
                [](char &c)
                {
                    c = ::tolower(c);
                });

            files[newFile.filename] = newFile;

            // printf("%lu: %s\n", files.size(), newFile.filename.c_str());
        }

        // printf("filesCount = %u \n", filesCount);
        return getErrorState();
    }

    cDATFile cDAT::getFileDataByName(std::string filename)
    {
        return files[filename];
    }

    unsigned char *cDAT::getFileByName(std::string filename)
    {
        std::string fname = filename;
        std::for_each(
            fname.begin(),
            fname.end(),
            [](char &c)
            {
                c = ::tolower(c);
            });

        if (input == nullptr)
            return nullptr;
        if (files.count(fname) == 0)
            return nullptr;

        unsigned char *dataBuffer = new unsigned char[files[fname].packedSize];
        fseek(input, files[fname].fileOffset, SEEK_SET);
        fread(dataBuffer, files[fname].packedSize, 1, input);

        if (files[fname].compressed == false)
        {
            return dataBuffer;
        }

        // printf("file is compressed!\n");

        unsigned char *uncompressedData = new unsigned char[files[fname].realSize];
        unsigned long destLen = files[fname].realSize;
        uncompress(uncompressedData, &destLen, dataBuffer, files[fname].packedSize);

        // printf("destLen = %lu, %u\n", destLen, files[fname].realSize );
        return uncompressedData;

        // 23139
    }

}