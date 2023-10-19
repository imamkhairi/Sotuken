#ifndef heightMap_CLASS_H
#define heightMap_CLASS_H

#include <opencv2/opencv.hpp>
#include <particleSystem.h>
#include <time.h>


const int avg[3][3] = {
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
};


class heightMap {
    public:
        heightMap(std::vector <Droplet> ParticleSystem, int mapHeight, int mapWidth);


    private:
        int mapHeight;
        int mapWidth;

        float calcHeight(Droplet a, int x_i, int y_i);
        void generateHeightMap(std::vector <Droplet> particleSystem);
        void smoothingHeightMap(cv::Mat heightMap);
        void checkCoordinate(int *x, int *y);
        void heightThreshold(float *value);

};

#endif