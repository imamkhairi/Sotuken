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

void heightMap::drawPatching(cv::Mat *dst) 
{
    for (int i : this->PSptr->patchingIndex) 
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
                    if ((*dst).at<unsigned char>(y0, x0) < h * 8)
                    {
                        (*dst).at<unsigned char>(y0, x0) = h * 8;
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
                        this->PSptr->checkIndex(this->idMapPtr->getIDMapValue(y0, x0), this->PSptr->mergingIndex))
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
    // this->idMapPtr->printInRange(x0, y0, x1, y1);

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
    int x0 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleLeft, MINVALUE) - 2;
    int y0 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleTop, MINVALUE) - 2;
    int x1 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleRight, MAXVALUE) + 2;
    int y1 = this->PSptr->getMergingCooridnate(&particleSystem::getParticleBottom, MAXVALUE) + 2;

    // std::cout << "start: " << "(" << x0 << ", " << y0 << ")" << std::endl;
    // std::cout << "end  : " << "(" << x1 << ", " << y1 << ")" << std::endl;


    cv::Size roiSize(x1-x0, y1-y0);
    
    cv::Mat mask = this->generateMergingMask(x0, y0, x1, y1, false);

    cv::Mat slicedImg = (*heightMap)(cv::Rect(cv::Point(x0, y0), roiSize));
    cv::Mat smoothingTarget = cv::Mat::zeros(slicedImg.size(), slicedImg.type());
    // slicedImg.copyTo(smoothingTarget);
    slicedImg.copyTo(smoothingTarget, mask);

    // for (int y = 0; y < roiSize.height; y++) 
    // {
    //     for (int x = 0; x < roiSize.width; x++)
    //     {
    //         if(mask.at<unsigned char>(y, x) > 0)
    //         {
    //             smoothingTarget.at<unsigned char>(y, x) = slicedImg.at<unsigned char>(y, x);
    //         }
    //     }
    // }




    cv::imwrite("before.png", slicedImg);
    cv::imwrite("after.png", smoothingTarget);
    cv::imwrite("mask.png", mask);

    cv::blur(smoothingTarget, smoothingTarget, cv::Size(3, 3), cv::Point(-1,-1), 0);
    cv::threshold(smoothingTarget, smoothingTarget, 8, 255, cv::THRESH_TOZERO);


    //// MASK SEPERTINYA BISA DIHAPUS
    for (int y = 0; y < roiSize.height; y++) 
    {
        for (int x = 0; x < roiSize.width; x++)
        {
            int idX = x + x0;
            int idY = y + y0;
            int idValue = this->idMapPtr->getIDMapValue(idY, idX);
            // if (smoothingTarget.at<unsigned char>(y, x)> 0)
            // if (this->idMapPtr->getIDMapValue(idY, idX) >= 0)
            // // && this->PSptr->checkMergingIndex(this->idMapPtr->getIDMapValue(y+y0, x+x0)))
            // {
            //     // if (mask.at<unsigned char>(y, x) > 0 && !this->PSptr->checkMergingIndex(this->idMapPtr->getIDMapValue(idY, idX)))
            //     if (mask.at<unsigned char>(y, x) > 0 || idMapPtr->getIDMapValue(idY, idX) == -1)
            //     {
            //         slicedImg.at<unsigned char>(y, x) = smoothingTarget.at<unsigned char>(y, x); 
            //         // slicedImg.at<unsigned char>(y, x) = 250; 
            //         // if (mask.at<unsigned char>(y, x) == 0)
            //         // {
            //         //     this->idMapPtr->setToValue(idY, idX, this->idMapPtr->getIDMapValue(idY, idX));
            //         // }
            //     }
            //     else 
            //     {
            //         if(this->PSptr->checkIndex(this->idMapPtr->getIDMapValue(idY, idX), this->PSptr->patchingIndex))
            //             this->PSptr->patchingIndex.push_back(this->idMapPtr->getIDMapValue(idY, idX));
            //     }
            // }
            if (idValue >= 0 && this->PSptr->checkIndex(idValue, this->PSptr->mergingIndex))
            {   
                if (this->PSptr->checkIndex(idValue, this->PSptr->patchingIndex))
                    this->PSptr->patchingIndex.push_back(idValue);
                continue;
            }
            if (smoothingTarget.at<unsigned char>(y, x) > 0) 
            {
                slicedImg.at<unsigned char>(y, x) = smoothingTarget.at<unsigned char>(y, x); 
                if (idValue < 0)
                {
                    if (this->idMapPtr->getIDMapValue(idY+1, idX) >= 0) 
                    {
                        this->idMapPtr->setToValue(idY, idX, this->idMapPtr->getIDMapValue(idY+1, idX));
                    } 
                    else if (this->idMapPtr->getIDMapValue(idY-1, idX) >= 0) 
                    {
                        this->idMapPtr->setToValue(idY, idX, this->idMapPtr->getIDMapValue(idY-1, idX));
                    }
                }
            }

        }
    }


    // this->idMapPtr->print();
    
    // smoothingTarget.copyTo(slicedImg, mask);



    mask.release();
    slicedImg.release();
    smoothingTarget.release();

    std::cout << "patching: ";
    for (int &i: this->PSptr->patchingIndex)
        std::cout << i << ", ";
    std::cout << std::endl;    
     
    this->drawPatching(heightMap);
    this->PSptr->patchingIndex.clear();
    // this->idMapPtr->printInRange(x0, y0, x1, y1);

}

cv::Mat heightMap::generateMergingMask(int x0, int y0, int x1, int y1, bool clearIdMap)
{
    cv::Mat mask (cv::Size(x1-x0, y1-y0), CV_8UC1);
    for (int y = y0; y < y1; y++) 
    {
        for (int x = x0; x < x1; x++)
        {
            // if (this->idMapPtr->getIDMapValue(y, x) >= 0)
            if (!this->PSptr->checkIndex(this->idMapPtr->getIDMapValue(y, x), this->PSptr->mergingIndex))
            {
                mask.at<unsigned char>(y - y0, x - x0) = 100;
                if (clearIdMap) this->idMapPtr->setToValue(y, x, -1);
            }
            else 
            {
                mask.at<unsigned char>(y - y0, x - x0) = 0;
            }
        }
    }
    return mask;
}