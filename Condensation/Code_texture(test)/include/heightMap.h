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
        heightMap(particleSystem *ParticleSystem, int mapHeight, int mapWidth);

        void updateHeightMap(particleSystem *PS);
        void smoothingHeightMap(particleSystem *PS);
        void smoothingHeightMap(IDMap idMap, particleSystem *PS);

    private:
        int mapHeight;
        int mapWidth;

        float calcHeight(Droplet a, int x_i, int y_i);
        void generateHeightMap(particleSystem *ParticleSystem);
        void checkCoordinate(int *x, int *y);
        int heightThreshold(float *value);
        void drawHeightMap(cv::Mat dst, std::vector <Droplet> particle, int start, int end);

};

#endif