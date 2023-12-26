#ifndef IDMap_CLASS_H
#define IDMap_CLASS_H

#include <cstdlib>

class IDMap {
    public:
        IDMap(int mapHeight, int mapWidth);
        
        void setToValue(int y, int x, int value);
        void print();
        void Delete();

    private:
        int *idMap;
        int mapHeight;
        int mapWidth;

        void initiateIDMap();
};

#endif