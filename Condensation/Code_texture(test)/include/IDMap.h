#ifndef IDMap_CLASS_H
#define IDMap_CLASS_H

#include <cstdlib>

class IDMap {
    public:
        IDMap(int mapHeight, int mapWidth);
        
        void setToValue(int y, int x, int value);
        void print();
        void Delete();

        int *getIDMap();
        int getIDMapValue(int y, int x);
        void clearSection(int y0, int x0, int y1, int x1);

    private:
        int *idMap;
        int mapHeight;
        int mapWidth;

        void initiateIDMap();
};

#endif