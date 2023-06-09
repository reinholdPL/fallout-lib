#include "cFrm.h"
#include "cDAT.h"

namespace falloutLib
{
    cFrm::cFrm()
    {
        orientationsStored = 6;
    }

    cFrm::~cFrm()
    {
        for (int orientation = 0; orientation < 6; orientation++)
        {
            for (int frame = 0; frame < frmFrames[orientation].size(); frame++)
            {
                delete frmFrames[orientation][frame];
            }
        }
    }

    cFrm::cFrm(const char *filename) : cFrm()
    {
        loadFrm(filename);
    }

    errorResult cFrm::loadFrm(const char *filename)
    {
        char *frmData = nullptr;
        unsigned long currentFileOffset = 0;
        fileContent fileRes = loadFileContentWithSize (filename);
        frmData = (char*)fileRes.buffer;

        sFILE* frmFile = datFopen(filename);

        if (frmFile == nullptr)
        {
            setErrorState(NO_FILE);
            return NO_FILE;
        }

        frmData = (char*)frmFile->buffer;
        

        version = readUInt32BEFromBuffer(frmData, 0x0000);
        if (version != 4)
        {
            setErrorState(FRM_VERSION_NOT_SUPPORTED);
            return FRM_VERSION_NOT_SUPPORTED;
        }

        fps = readUInt16BEFromBuffer(frmData, 0x0004);
        actionFrame = readUInt16BEFromBuffer(frmData, 0x0006);
        noOfFramesPerDirection = readUInt16BEFromBuffer(frmData, 0x0008);

        currentFileOffset = 0x000A;
        for (unsigned int direction = 0; direction < 2; direction++)
        { // dir 0=x, 1=y
            for (unsigned int orientation = 0; orientation < 6; orientation++)
            {
                pixelShiftDirection[direction].orientation[orientation] = readUInt16BEFromBuffer(frmData, 0x0006);
                currentFileOffset += 2;
            }
        }


        for (unsigned int orientation = 0; orientation < 6; orientation++)
        {
            unsigned int tmp = readUInt32BEFromBuffer(frmData, currentFileOffset);
            offsetForFrameArea[orientation] = readUInt32BEFromBuffer(frmData, currentFileOffset);
            // printf("%X: area currentFileOffset = %d for orient %d %d TMP = \n",currentFileOffset, offsetForFrameArea[orientation], orientation, tmp);
            currentFileOffset += 4;
        }

        sizeOfFrameData = readUInt32BEFromBuffer(frmData, currentFileOffset);
        currentFileOffset += 4;

        // reading frame area
        for (unsigned int orientation = 0; orientation < 6; orientation++)
        {
            currentFileOffset = offsetForFrameArea[orientation] + 0x003E; //+ start of frame area
            if (orientation > 0 && offsetForFrameArea[orientation] == 0)
            {
                orientationsStored = 1;
                break;
            }

            for (unsigned int frame = 0; frame < noOfFramesPerDirection; frame++)
            {
                unsigned short width, height;
                short pixelShiftX, pixelShiftY;
                char *pixelData = nullptr;
                cFrmFrame *newFrame = new cFrmFrame();

                width = readUInt16BEFromBuffer(frmData, currentFileOffset);
                newFrame->setWidth(width);
                height = readUInt16BEFromBuffer(frmData, currentFileOffset + 2);
                newFrame->setHeight(height);

                // printf("orientation = %d, frame %d/%d %dx%d currentFileOffset=%ld fsize=%ld\n", orientation, frame, noOfFramesPerDirection, width, height, currentFileOffset, fileRes.size);

                currentFileOffset += 4;
                currentFileOffset += 4; // skipping frame-size

                pixelShiftX = readUInt16BEFromBuffer(frmData, currentFileOffset);
                pixelShiftY = readUInt16BEFromBuffer(frmData, currentFileOffset + 2);
                newFrame->setPixelShift(0, pixelShiftX);
                newFrame->setPixelShift(1, pixelShiftY);
                currentFileOffset += 4;

                // printf("currentFileOff = %x\n", currentFileOffset);
                pixelData = new char[width * height];
                memcpy(pixelData, frmData + currentFileOffset, width * height);

                newFrame->setPixelData(pixelData);

                currentFileOffset += width * height;
                frmFrames[orientation].push_back(newFrame);
            }
        }




        free(frmData);
        return NO_ERROR;
    }

    cFrmFrame *cFrm::getFrame(unsigned int direction, unsigned int frameNum)
    {
        if (frmFrames[direction].size() > frameNum)
        {
            return frmFrames[direction][frameNum];
        }

        return nullptr;
    }
}
