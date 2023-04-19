#include "cMap.h"

namespace falloutLib
{
    cMap::cMap(const char *filename)
    {
        loadMap(filename);
    }

    errorResult cMap::loadMap(const char *filename)
    {
        setErrorState(NO_ERROR);

        char *mapData = nullptr;
        unsigned long currentFileOffset = 0;
        mapData = loadFileContent(filename);

        if (mapData == nullptr)
        {
            setErrorState(NO_FILE);
            return NO_FILE;
        }

        mapVersion = readUInt32BEFromBuffer(mapData, 0);
        printf("mapVersion = %u\n", mapVersion);

        memcpy(mapName, mapData + 4, 16);
        printf("mapName = %s\n", mapName);

        defaultPlayerPosition = readUInt32BEFromBuffer(mapData, 0x0014);
        defaultMapElevation = readUInt32BEFromBuffer(mapData, 0x0018);
        defaultPlayerOrientation = readUInt32BEFromBuffer(mapData, 0x001C);
        noOfLocalVars = readUInt32BEFromBuffer(mapData, 0x0020);
        mapScriptId = readUInt32BEFromBuffer(mapData, 0x0024);
        mapFlags = readUInt32BEFromBuffer(mapData, 0x0028);

        noOfGlobalVars = readUInt32BEFromBuffer(mapData, 0x0030);
        mapId = readUInt32BEFromBuffer(mapData, 0x0034);
        timeSinceEpoch = readUInt32BEFromBuffer(mapData, 0x0038);

        currentFileOffset = 0x00EC + 4 * (noOfGlobalVars + noOfLocalVars);

        for (unsigned int i = 0; i < 3; i++)
        {
            if (hasElevation(i))
            {
                elevationsInMap.push_back(i);
                // read elevation data
                cMapElevation newElevation;

                unsigned short tmpRoof, tmpFloor;

                for (unsigned int j = 0; j < 100; j++)
                {
                    for (unsigned int i = 0; i < 100; i++)
                    {
                        tmpRoof = readUInt16BEFromBuffer(mapData, currentFileOffset);
                        tmpFloor = readUInt16BEFromBuffer(mapData, currentFileOffset + 2);
                        newElevation.setRoofTile(i, j, tmpRoof);
                        newElevation.setFloorTile(i, j, tmpFloor);
                        currentFileOffset += 4;
                    }
                }

                elevations[i] = newElevation;
            }
        }

        free(mapData);
        return NO_ERROR;
    }

    bool cMap::isSaveFile()
    {
        if ((mapFlags & 0x1) != 0)
            return true;
        else
            return false;
    }

    bool cMap::hasElevation(unsigned int elev)
    {
        if ((mapFlags & (1 << (elev + 1))) == 0)
            return true;
        else
            return false;
    }

    void cMapElevation::setFloorTile(unsigned int x, unsigned int y, unsigned short value)
    {
        floorTiles[x][y] = value;
    }
    void cMapElevation::setRoofTile(unsigned int x, unsigned int y, unsigned short value)
    {
        roofTiles[x][y] = value;
    }

    unsigned short cMapElevation::getFloorTile(unsigned int x, unsigned int y)
    {
        return floorTiles[x][y];
    }
    unsigned short cMapElevation::getRoofTile(unsigned int x, unsigned int y)
    {
        return roofTiles[x][y];
    }

}