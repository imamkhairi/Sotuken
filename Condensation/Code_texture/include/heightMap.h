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
    
        void smoothingHeightMap(IDMap idMap, particleSystem *PS);
        void updateHeightMap(particleSystem *PS);

    private:
        int mapHeight;
        int mapWidth;

        cv::Mat heightMapMat;

        float calcHeight(Droplet a, int x_i, int y_i);
        void generateHeightMap(particleSystem *ParticleSystem);
        // void smoothingHeightMap(cv::Mat heightMap);
        void checkCoordinate(int *x, int *y);
        int heightThreshold(float *value);
        void drawHeightMap(cv::Mat dst, std::vector <Droplet> particle, int start, int end);
        float distance(float x1, float y1, float x2, float y2);

};

#endif