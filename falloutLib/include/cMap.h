#ifndef __C_MAP_H
#define __C_MAP_H

#include <utils.h>
#include <cError.h>
#include <cstring>
#include <vector>
#include <map>

namespace falloutLib
{
    class cMapElevation
    {
    public:
        unsigned short getFloorTile(unsigned int x, unsigned int y);
        unsigned short getRoofTile(unsigned int x, unsigned int y);
        void setFloorTile(unsigned int x, unsigned int y, unsigned short value);
        void setRoofTile(unsigned int x, unsigned int y, unsigned short value);

    private:
        unsigned short floorTiles[100][100];
        unsigned short roofTiles[100][100];
    };

    class cMap : public cError
    {
    public:
        cMap();
        cMap(const char *filename);
        errorResult loadMap(const char *filename);
        bool isSaveFile();
        bool hasElevation(unsigned int elev);
        cMapElevation getElevation(unsigned int elevation);

    private:
        unsigned int mapVersion;
        char mapName[16];
        int defaultPlayerPosition;
        int defaultMapElevation;
        int defaultPlayerOrientation;
        int noOfLocalVars;
        int noOfGlobalVars;
        int mapScriptId;
        int mapFlags;
        int mapId;
        unsigned int timeSinceEpoch;
        std::vector<unsigned int> elevationsInMap;
        std::map<unsigned int, cMapElevation> elevations;
    };
}

#endif