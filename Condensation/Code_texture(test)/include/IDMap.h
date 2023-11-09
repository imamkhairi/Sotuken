#ifndef IDMap_CLASS_H
#define IDMap_CLASS_H

#include <cstdlib>

class IDMap {
    public:
        IDMap(int mapHeight, int mapWidth);
        
        void setToOne(int y, int x);

    private:
        int *idMap;
        int mapHeight;
        int mapWidth;

        void initiateIDMap();
};

#endif