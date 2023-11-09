#include <IDMap.h>

IDMap::IDMap(int mapWidth, int mapHeight) {
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->idMap = (int *)malloc(mapWidth * mapHeight * sizeof(int));

    this->initiateIDMap();
}

void IDMap::initiateIDMap() {
    int *p;
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            *p = 0;
            p++;
        }
    }
}

void IDMap::setToOne(int y, int x) {
    this->idMap[y*mapWidth + x] = 1;
}
