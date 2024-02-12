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
        heightMap(particleSystem *ParticleSystem, IDMap *idMapPtr, int mapHeight, int mapWidth);
        cv::Mat getHeightMap();
        void generateHeightMap();

    private:
        int mapHeight;
        int mapWidth;
        bool initiated;
        cv::Mat heightMapMat;
        cv::Mat smoothedHeightMapMat;
        IDMap *idMapPtr;
        particleSystem *psPtr;

        float calcHeight(Droplet a, int x_i, int y_i);
        void drawHeightMap(cv::Mat dst, std::vector <Droplet> *particlePtr, std::vector <int> *updatedParticlesPtr);
        float distance(float x1, float y1, float x2, float y2);

        void clearHeight();
        void drawMergingOrPatching(std::vector<int> *index);
        void smoothingMerging();

        cv::Mat generateMergingMask(int x0, int y0, int x1, int y1, bool clearIdMap);

};

#endif