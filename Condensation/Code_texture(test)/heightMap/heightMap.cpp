#include <heightMap.h>

heightMap::heightMap(particleSystem *ParticleSystem, IDMap *idMapPtr, int mapHeight, int mapWidth, int smCount) {
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->idMapPtr = idMapPtr;
    this->PSptr = ParticleSystem;
    this->smCount = smCount;
    
    // clock_t tStart = clock();
    this->generateHeightMap();
    // printf("Raw: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));
}

float heightMap::distance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x1-x2, 2) + std::pow(y1-y2,2));
}

void heightMap::drawMerging(cv::Mat dst) 
{
    for (int i : this->PSptr->mergingIndex) 
    {
        // starting point
        int x = PSptr->getParticleSystem()[i].position.x - (int)PSptr->getParticleSystem()[i].radius - 1;
        int y = PSptr->getParticleSystem()[i].position.y - (int)PSptr->getParticleSystem()[i].radius - 1;
        // end point
        int x1 = PSptr->getParticleSystem()[i].position.x + (int)PSptr->getParticleSystem()[i].radius + 1;
        int y1 = PSptr->getParticleSystem()[i].position.y + (int)PSptr->getParticleSystem()[i].radius + 1;

        for (int y0 = y; y0 <= y1; y0++) 
        {
            for (int x0 = x; x0 <= x1; x0++) 
            {
                float h = calcHeight(PSptr->getParticleSystem()[i], x0, y0);
                if ( h > 0.01 ) 
                {
                    if (dst.at<unsigned char>(y0, x0) < h * 8)
                    {
                        dst.at<unsigned char>(y0, x0) = h * 8;
                        this->idMapPtr->setToValue(y0, x0, i);
                    }
                }
            }
        }
    }
}

void heightMap::drawHeightMap(cv::Mat dst, std::vector <Droplet> PS, int start, int end) 
{
    for (int i = start; i < end; i++) 
    {
        // starting point
        int x = PS[i].position.x - (int)PS[i].radius - 1;
        int y = PS[i].position.y - (int)PS[i].radius - 1;
        // end point
        int x1 = PS[i].position.x + (int)PS[i].radius + 1;
        int y1 = PS[i].position.y + (int)PS[i].radius + 1;

        bool mergingFlag = false;

        for (int y0 = y; y0 <= y1; y0++) 
        {
            for (int x0 = x; x0 <= x1; x0++) 
            {
                float h = calcHeight(PS[i], x0, y0);
                if ( h > 0.01 ) 
                {
                    if (dst.at<unsigned char>(y0, x0) < h * 8)
                    {
                        dst.at<unsigned char>(y0, x0) = h * 8; // 50 konstansta
                        // std::cout << h * 20 << std::endl;
                        if(this->idMapPtr->getIDMapValue(y0, x0) >= 0 && this->idMapPtr->getIDMapValue(y0, x0) != i &&
                        this->PSptr->checkMergingIndex(this->idMapPtr->getIDMapValue(y0, x0)))
                        {
                            this->PSptr->mergingIndex.push_back(this->idMapPtr->getIDMapValue(y0, x0));
                            mergingFlag = true;
                        }
                        this->idMapPtr->setToValue(y0, x0, i);
                    }
                }
            }
        }

        if (mergingFlag) 
        {
            this->PSptr->mergingIndex.push_back(i);
            this->clearHeight(&dst);
            // this->PSptr->printMergingIndex(); // debug
            this->PSptr->updateMergingMass();
            this->drawMerging(dst);
            this->smoothingMerging(&dst);
            this->PSptr->mergingIndex.clear();
        }
    }
}

// need optimization
void heightMap::generateHeightMap() {
    cv::Mat heightMap = cv::Mat::zeros(this->mapWidth, this->mapHeight, CV_8UC1);

    // Old method
    // for (unsigned int y = 0; y < this->mapHeight; y++) {
    //     for (unsigned int x = 0; x < this->mapWidth; x++) {
    //         // heightMap.at<unsigned char>(y, x) = h*(255/r); // 255/r ini asumsi max 255
    //         for (auto & particle : particleSystem) {
    //             std::cout << "radius :" << particle.radius << std::endl;
    //             float h = calcHeight(particle, x, y);
    //             if ( h > 0.0 ) {
    //                 // std::cout << "h value : " << h * 50 << std::endl;
    //                 heightMap.at<unsigned char>(y, x) = h*50; // 50 konstansta
    //             }
    //         }
    //     }
    // }

    std::vector <Droplet> particle =  this->PSptr->getParticleSystem(); // bisa pake auto
    int start = this->PSptr->getDrewAmmount();
    int end = this->PSptr->getParticleAmmount();
    this->drawHeightMap(heightMap, particle, start, end);

    this->PSptr->setDrewAmmount(this->PSptr->getParticleAmmount());

    // this->smoothingHeightMap(heightMap);
    for (int i = 0; i < this->smCount; i++) 
    {
        cv::blur(heightMap, heightMap, cv::Size(3, 3));
        cv::threshold(heightMap, heightMap, 8, 255, cv::THRESH_TOZERO);
    }

    cv::imwrite("heightMap.png", heightMap);
}

float heightMap::calcHeight(Droplet a, int x_i, int y_i) {
    glm::vec2 dp(a.position.x - x_i, a.position.y - y_i);
    float value = a.radius*a.radius - glm::length(dp)*glm::length(dp);
    float height;
    if (value > 0) 
    {
        height = std::sqrt(value);
        return height;
    } else return 0;
}

void heightMap::clearHeight(cv::Mat *heightMap) 
{
    int x0 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleLeft, MINVALUE) - 1;
    int y0 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleTop, MINVALUE) - 1;
    int x1 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleRight, MAXVALUE) + 2;
    int y1 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleBottom, MAXVALUE) + 2;
    
    cv::Size roiSize(x1-x0, y1-y0);
    cv::Mat mask = this->generateMergingMask(x0, y0, x1, y1, true);
    cv::Mat slicedImg = (*heightMap)(cv::Rect(cv::Point(x0, y0), roiSize));
    cv::Mat black = cv::Mat::zeros(slicedImg.size(), slicedImg.type());
    black.copyTo(slicedImg, mask);

    mask.release();
    slicedImg.release();
    black.release();
}

void heightMap::smoothingMerging(cv::Mat *heightMap) 
{
    this->PSptr->printMergingIndex();
    int x0 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleLeft, MINVALUE) - 1;
    int y0 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleTop, MINVALUE) - 1;
    int x1 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleRight, MAXVALUE) + 2;
    int y1 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleBottom, MAXVALUE) + 2;

    std::cout << "start: " << "(" << x0 << ", " << y0 << ")" << std::endl;
    std::cout << "end  : " << "(" << x1 << ", " << y1 << ")" << std::endl;


    std::cout << std::endl;

    cv::Size roiSize(x1-x0, y1-y0);
    
    cv::Mat mask = this->generateMergingMask(x0, y0, x1, y1, false);

    cv::Mat slicedImg = (*heightMap)(cv::Rect(cv::Point(x0, y0), roiSize));
    cv::Mat smoothingTarget = cv::Mat(slicedImg.size(), slicedImg.type());
    slicedImg.copyTo(smoothingTarget);
    cv::blur(smoothingTarget, smoothingTarget, cv::Size(3, 3));
    smoothingTarget.copyTo(slicedImg, mask);
    cv::imwrite("test.png", slicedImg);
    cv::imwrite("mask.png", mask);

    mask.release();
    slicedImg.release();
    smoothingTarget.release();
}

cv::Mat heightMap::generateMergingMask(int x0, int y0, int x1, int y1, bool clearIdMap)
{
    cv::Mat mask = cv::Mat::zeros(cv::Size(x1-x0, y1-y0), CV_8UC1);
    for (int y = y0; y < y1; y++) 
    {
        for (int x = x0; x < x1; x++)
        {
            // if (this->idMapPtr->getIDMapValue(y, x) >= 0)
            if (!this->PSptr->checkMergingIndex(this->idMapPtr->getIDMapValue(y, x)))
            {
                mask.at<unsigned char>(y - y0, x - x0) = 100;
                if (clearIdMap) this->idMapPtr->setToValue(y, x, -1);
            }
        }
    }
    return mask;
}