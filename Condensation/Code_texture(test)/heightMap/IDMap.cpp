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
            std::cout << std::setw(2) << *p << " ";
            p++;
        }
        std::cout << std::endl;
    }
}

void IDMap::printInRange(int x0, int y0, int x1, int y1) 
{
    for (int y = y0; y < y1; y++) 
    {
        for (int x = x0; x < x1; x++)
        {
            std::cout << std::setw(2) << this->getIDMapValue(y, x) << " ";
        }
        std::cout << std::endl;
    }
}

void IDMap::Delete() {
    free(this->idMap);
}

int *IDMap::getIDMap() 
{
    return this->idMap;
}

int IDMap::getIDMapValue(int y, int x) 
{
    return this->idMap[y*this->mapWidth + x];
}

void IDMap::clearSection(int y0, int x0, int y1, int x1) 
{
    for (int y = y0; y < y1; y++) 
    {
        for (int x = x0; x < x1; x++) 
        {
            this->setToValue(y, x, -1);
        }
    }
}