#include <heightMap.h>
#include <time.h>

heightMap::heightMap(particleSystem *ParticleSystem, IDMap *idMapPtr, int mapHeight, int mapWidth)
{
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->initiated = false;
    this->idMapPtr = idMapPtr;
    this->psPtr = ParticleSystem;
    this->generateHeightMap();
}

float heightMap::distance(float x1, float y1, float x2, float y2)
{
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

void heightMap::drawHeightMap(cv::Mat dst, std::vector<Droplet> *PS, std::vector <int> *updatedParticlesPtr)
{
    for (auto &i : *updatedParticlesPtr)
    {
        // starting point
        int x = (*PS)[i].position.x - (int)(*PS)[i].radius;
        int y = (*PS)[i].position.y - (int)(*PS)[i].radius;
        // end point
        int x1 = (*PS)[i].position.x + (int)(*PS)[i].radius;
        int y1 = (*PS)[i].position.y + (int)(*PS)[i].radius;

        bool mergingFlag = false;

        for (int y0 = y; y0 <= y1; y0++)
        {
            for (int x0 = x; x0 <= x1; x0++)
            {
                float h = calcHeight((*PS)[i], x0, y0);
                if (h > 0)
                {
                    if (dst.at<unsigned char>(y0, x0) < h * 8) {
                        dst.at<unsigned char>(y0, x0) = h * 8; // 50 konstansta
                        
                        // if (this->idMapPtr->getIDvalue(y0, x0) >= 0) {}
                        if(this->idMapPtr->getIDvalue(y0, x0) >= 0 && this->idMapPtr->getIDvalue(y0, x0) != i &&
                        this->psPtr->checkIndex(this->idMapPtr->getIDvalue(y0, x0), this->psPtr->getMergingIndex()))
                        {
                            this->psPtr->getMergingIndex().push_back(this->idMapPtr->getIDvalue(y0, x0));
                            mergingFlag = true;
                            // std::cout << "overlapping " << this->idMapPtr->getIDvalue(y0, x0) << ", " << i << std::endl ;
                        }
                        
                        this->idMapPtr->setToValue(y0, x0, i);
                    }
                }
            }
        }

        if (mergingFlag)
        {
            this->psPtr->getMergingIndex().push_back(i);
            this->clearHeight();
            this->psPtr->updateMergingMass();
            this->drawMergingOrPatching(&this->psPtr->getMergingIndex());
            this->smoothingMerging();
            this->psPtr->getMergingIndex().clear();
        }
    }
    
    (*updatedParticlesPtr).clear();
}

void heightMap::generateHeightMap()
{
    if (!this->initiated)
    {
        this->heightMapMat = cv::Mat::zeros(this->mapWidth, this->mapHeight, CV_8UC1);
        this->initiated = true;
    }

    std::vector<Droplet> *particle = &this->psPtr->getParticleSystem(); // bisa pake auto
    this->drawHeightMap(this->heightMapMat, particle, &this->psPtr->getUpdatedParticles());

    // clock_t tStart = clock();
    cv::blur(this->heightMapMat, this->smoothedHeightMapMat, cv::Size(5, 5), cv::Point(-1,-1), 0);

    cv::threshold(this->smoothedHeightMapMat, this->smoothedHeightMapMat, 8, 255, cv::THRESH_TOZERO);
    // printf("Smoothing and tresholding: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));
}

float heightMap::calcHeight(Droplet a, int x_i, int y_i)
{
    glm::vec2 dp(a.position.x - x_i, a.position.y - y_i);
    float value = a.radius * a.radius - glm::length(dp) * glm::length(dp);
    float height;
    if (value > 0)
    {
        height = std::sqrt(value);
        return height;
    }
    else
        return 0;
}

cv::Mat heightMap::getHeightMap()
{
    return this->smoothedHeightMapMat;
    // return this->heightMapMat;
}

void heightMap::clearHeight()
{
    int x0 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleLeft, MINVALUE) - 2;
    int y0 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleTop, MINVALUE) - 2;
    int x1 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleRight, MAXVALUE) + 2;
    int y1 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleBottom, MAXVALUE) + 2;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) x0 = 0;
    if (x1 > this->mapWidth) x1 = this->mapWidth;
    if (y1 > this->mapWidth) y1 = this->mapHeight;

    if (x0 < 0 || y0 < 0) std::cout << "clear offside awal" << std::endl;
    if (x1 > this->mapWidth || y1 > this->mapHeight) std::cout << "clear offside akhir" << std::endl;

    cv::Size roiSize(x1-x0, y1-y0);

    cv::Mat mask = this->generateMergingMask(x0, y0, x1, y1, true);
    cv::Mat slicedImg = this->heightMapMat(cv::Rect(cv::Point(x0, y0), roiSize));
    cv::Mat black = cv::Mat::zeros(slicedImg.size(), slicedImg.type());
    black.copyTo(slicedImg, mask);

    mask.release();
    slicedImg.release();
    black.release();
}

// ini harusnya juga bisa dipake buat patching pake draw yang gede jg bisa tinggal bool buat cek merging
void heightMap::drawMergingOrPatching(std::vector<int> *index)
{
    for (int &i: (*index))
    {
        // starting point
        int x = psPtr->getParticleSystem()[i].position.x - (int)psPtr->getParticleSystem()[i].radius - 1;
        int y = psPtr->getParticleSystem()[i].position.y - (int)psPtr->getParticleSystem()[i].radius - 1;
        // end point
        int x1 = psPtr->getParticleSystem()[i].position.x + (int)psPtr->getParticleSystem()[i].radius + 1;
        int y1 = psPtr->getParticleSystem()[i].position.y + (int)psPtr->getParticleSystem()[i].radius + 1;

        for (int y0 = y; y0 <= y1; y0++) 
        {
            for (int x0 = x; x0 <= x1; x0++) 
            {
                float h = calcHeight(psPtr->getParticleSystem()[i], x0, y0);
                if ( h > 0.01 ) 
                {
                    if (this->heightMapMat.at<unsigned char>(y0, x0) < h * 8)
                    {
                        this->heightMapMat.at<unsigned char>(y0, x0) = h * 8;
                        this->idMapPtr->setToValue(y0, x0, i);
                    }
                }
            }
        }
    }
}

void heightMap::smoothingMerging() 
{
    int x0 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleLeft, MINVALUE) - 2;
    int y0 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleTop, MINVALUE) - 2;
    int x1 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleRight, MAXVALUE) + 2;
    int y1 = this->psPtr->getMergingCoordinate(&particleSystem::getParticleBottom, MAXVALUE) + 2;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) x0 = 0;
    if (x1 > this->mapWidth) x1 = this->mapWidth ;
    if (y1 > this->mapWidth) y1 = this->mapHeight;

    if (x0 < 0 || y0 < 0) std::cout << "smoothing offside awal" << std::endl;
    if (x1 > this->mapWidth || y1 > this->mapHeight) std::cout << "smoothing offside akhir" << std::endl;

    cv::Size roiSize(x1-x0, y1-y0);
    
    cv::Mat mask = this->generateMergingMask(x0, y0, x1, y1, false);

    cv::Mat slicedImg = this->heightMapMat(cv::Rect(cv::Point(x0, y0), roiSize));
    cv::Mat smoothingTarget = cv::Mat::zeros(slicedImg.size(), slicedImg.type());
    slicedImg.copyTo(smoothingTarget, mask);

    cv::blur(smoothingTarget, smoothingTarget, cv::Size(3, 3), cv::Point(-1,-1), 0);
    cv::threshold(smoothingTarget, smoothingTarget, 8, 255, cv::THRESH_TOZERO);

    // copy to origin
    for (int y = 0; y < roiSize.height; y++) 
    {
        for (int x = 0; x < roiSize.width; x++)
        {
            int idX = x + x0;
            int idY = y + y0;
            int idValue = this->idMapPtr->getIDvalue(idY, idX);

            if (idValue >= 0 && this->psPtr->checkIndex(idValue, this->psPtr->getMergingIndex()))
            {   
                if (this->psPtr->checkIndex(idValue, this->psPtr->getPatchingIndex()))
                    this->psPtr->getPatchingIndex().push_back(idValue);
                continue;
            }
            if (smoothingTarget.at<unsigned char>(y, x) > 0) 
            {
                slicedImg.at<unsigned char>(y, x) = smoothingTarget.at<unsigned char>(y, x); 
                if (idValue < 0)
                {
                    if (this->idMapPtr->getIDvalue(idY+1, idX) >= 0) 
                    {
                        this->idMapPtr->setToValue(idY, idX, this->idMapPtr->getIDvalue(idY+1, idX));
                    } 
                    else if (this->idMapPtr->getIDvalue(idY-1, idX) >= 0) 
                    {
                        this->idMapPtr->setToValue(idY, idX, this->idMapPtr->getIDvalue(idY-1, idX));
                    }
                }
            }

        }
    }

    mask.release();
    slicedImg.release();
    smoothingTarget.release();

    this->drawMergingOrPatching(&this->psPtr->getPatchingIndex());
    this->psPtr->getPatchingIndex().clear();
}


cv::Mat heightMap::generateMergingMask(int x0, int y0, int x1, int y1, bool clearIdMap) 
{
    cv::Mat mask = cv::Mat::zeros(cv::Size(x1-x0, y1-y0), CV_8UC1);
    for (int y = y0; y < y1; y++) 
    {
        for (int x = x0; x < x1; x++)
        {
            if (!this->psPtr->checkIndex(this->idMapPtr->getIDvalue(y, x), this->psPtr->getMergingIndex()))
            {
                mask.at<unsigned char>(y - y0, x - x0) = 100;
                if (clearIdMap) this->idMapPtr->setToValue(y, x, -1);
            }
        }
    }
    return mask;
}