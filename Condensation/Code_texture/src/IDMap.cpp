#include <IDMap.h>
#include <iostream>
#include <iomanip>

IDMap::IDMap(int mapWidth, int mapHeight) {
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
    this->idMap = (int *)malloc(mapWidth * mapHeight * sizeof(int));

    this->initiateIDMap();
}

void IDMap::initiateIDMap() {
    int *p = this->idMap;
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            *p = -1;
            p++;
        }
    }
}

void IDMap::setToValue(int y, int x, int value) {
    this->idMap[y*mapWidth + x] = value;
}

void IDMap::print() {
    int *p = this->idMap;
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            std::cout << std::setw(3) << *p << " ";
            p++;
        }
        std::cout << std::endl;
    }
}

void IDMap::Delete() 
{
    free(this->idMap);
}

int IDMap::getIDvalue(int y, int x)
{
    return this->idMap[y*this->mapWidth + x];
}
