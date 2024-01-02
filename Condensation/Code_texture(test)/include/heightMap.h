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
        heightMap(particleSystem *ParticleSystem, IDMap *idMapPtr, int mapHeight, int mapWidth, int smCount);

        void generateHeightMap();

    private:
        int mapHeight;
        int mapWidth;
        int smCount;
        IDMap *idMapPtr;
        particleSystem *PSptr;

        float calcHeight(Droplet a, int x_i, int y_i);
        void drawHeightMap(cv::Mat dst, std::vector <Droplet> particle, int start, int end);
        float distance(float x1, float y1, float x2, float y2);
        void clearHeight(int i, cv::Mat *heightMap);
        void drawMerging(cv::Mat dst);

};

#endif