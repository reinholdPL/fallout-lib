#include "cFrm.h"

namespace falloutLib
{
    cFrm::cFrm()
    {
    }

    cFrm::cFrm(const char *filename) : cFrm()
    {
        loadFrm(filename);
    }

    errorResult cFrm::loadFrm(const char *filename)
    {
        char *frmData = nullptr;
        unsigned long currentFileOffset = 0;
        frmData = loadFileContent(filename);

        if (frmData == nullptr)
        {
            setErrorState(NO_FILE);
            return NO_FILE;
        }

        version = readUInt32BEFromBuffer(frmData, 0x0000);
        if (version != 4)
        {
            setErrorState(FRM_VERSION_NOT_SUPPORTED);
            return FRM_VERSION_NOT_SUPPORTED;
        }

        fps = readUInt16BEFromBuffer(frmData, 0x0004);
        actionFrame = readUInt16BEFromBuffer(frmData, 0x0006);
        noOfFramesPerDirection = readUInt16BEFromBuffer(frmData, 0x0008);

        
        free(frmData);
        return NO_ERROR;
    }

}