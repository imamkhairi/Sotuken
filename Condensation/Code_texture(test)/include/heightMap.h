#ifndef heightMap_CLASS_H
#define heightMap_CLASS_H

#include <opencv2/opencv.hpp>
#include <particleSystem.h>
#include <IDMap.h>


const int avg[3][3] = {
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
};


class heightMap {
    public:
        heightMap(particleSystem *ParticleSystem, IDMap *idMap, int mapHeight, int mapWidth);

        void updateHeightMap(particleSystem *PS);
        void smoothingHeightMap(particleSystem *PS);
        void smoothingHeightMap(IDMap idMap, particleSystem *PS);

        void generateHeightMap(particleSystem *ParticleSystem, IDMap *idMap);

    private:
        int mapHeight;
        int mapWidth;

        float calcHeight(Droplet a, int x_i, int y_i);
        void checkCoordinate(int *x, int *y);
        void checkXY(int *x, int *y);
        int heightThreshold(float *value);
        void drawHeightMap(cv::Mat dst, std::vector <Droplet> particle, IDMap *idMap, int start, int end);
        float distance(float x1, float y1, float x2, float y2);

};

#endif