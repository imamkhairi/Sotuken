#ifndef heightMap_CLASS_H
#define heightMap_CLASS_H

#include <opencv2/opencv.hpp>
#include <particleSystem.h>


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

        void generateHeightMap(std::vector <Droplet> particleSystem);
        float calcHeight(Droplet a, int x_i, int y_i);

};

#endif