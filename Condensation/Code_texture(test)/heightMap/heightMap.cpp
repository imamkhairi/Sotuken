#include <heightMap.h>

heightMap::heightMap(particleSystem *ParticleSystem, IDMap *idMapPtr, int mapHeight, int mapWidth, int smCount) {
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->idMapPtr = idMapPtr;
    this->smCount = smCount;
    this->generateHeightMap(ParticleSystem);
}

float heightMap::distance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x1-x2, 2) + std::pow(y1-y2,2));
}

void heightMap::drawHeightMap(cv::Mat dst, std::vector <Droplet> PS, int start, int end) {
    for (int i = start; i < end; i++) {
        // starting point
        int x = PS[i].position.x - (int)PS[i].radius - 1;
        int y = PS[i].position.y - (int)PS[i].radius - 1;
        // end point
        int x1 = PS[i].position.x + (int)PS[i].radius + 1;
        int y1 = PS[i].position.y + (int)PS[i].radius + 1;

        for (int y0 = y; y0 <= y1; y0++) {
            for (int x0 = x; x0 <= x1; x0++) {
                float h = calcHeight(PS[i], x0, y0);
                if ( h > 0.01 ) {
                    if (dst.at<unsigned char>(y0, x0) < h * 8)
                    {
                        dst.at<unsigned char>(y0, x0) = h * 8; // 50 konstansta
                        // std::cout << h * 20 << std::endl;
                        if(this->idMapPtr->getIDMapValue(y0, x0) >= 0 && this->idMapPtr->getIDMapValue(y0, x0) != i)
                            std::cout << "Merging (" << i << "," << this->idMapPtr->getIDMapValue(y0,x0) << ")" << std::endl;
                        this->idMapPtr->setToValue(y0, x0, i);
                    }
                }
            }
        }
    }

    //// Metaball
    // for (int y = 0; y < this->mapHeight; y++) {
    //     for (int x = 0; x < this->mapWidth; x++) {
    //         float d = 0, sum = 0;

    //         for (auto & particle : PS) {
    //             d = this->distance(particle.position.x, particle.position.y, x, y);
    //             sum += 1 / (1 + std::pow((d/particle.radius), 2.8));
    //         }
    //         sum -= 0.5;
    //         if (sum > 0) sum *= 255;
    //         dst.at<unsigned char>(y, x) = sum;
    //     }
    // }
}

// need optimization
void heightMap::generateHeightMap(particleSystem *PS) {
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

    std::vector <Droplet> particle =  PS->getParticleSystem(); // bisa pake auto
    int start = PS->getDrewAmmount();
    int end = PS->getParticleAmmount();
    this->drawHeightMap(heightMap, particle, start, end);

    PS->setDrewAmmount(PS->getParticleAmmount());

    // this->smoothingHeightMap(heightMap);
    for (int i = 0; i < this->smCount; i++) {
        cv::blur(heightMap, heightMap, cv::Size(3, 3));
        cv::threshold(heightMap, heightMap, 8, 255, cv::THRESH_TOZERO);
    }

    cv::imwrite("heightMap.png", heightMap);
}

float heightMap::calcHeight(Droplet a, int x_i, int y_i) {
    glm::vec2 dp(a.position.x - x_i, a.position.y - y_i);
    float value = a.radius*a.radius - glm::length(dp)*glm::length(dp);
    float height;
    if (value > 0) {
        height = std::sqrt(value);
        return height;
    } else return 0;
}

void heightMap::smoothingHeightMap(particleSystem *PS) {
    cv::Mat heightMap = cv::imread("heightMap.png", cv::IMREAD_GRAYSCALE);
    cv::Mat smoothed  = cv::Mat::zeros(this->mapHeight, this->mapHeight, CV_8UC1);

    std::vector <Droplet> particle =  PS->getParticleSystem(); // bisa pake auto
    for (auto & p : particle) {
        // starting point
        int x = p.position.x - (int)p.radius - 2;
        int y = p.position.y - (int)p.radius - 2;
        // end point
        int x1 = p.position.x + (int)p.radius + 2;
        int y1 = p.position.y + (int)p.radius + 2;


        for (int y0 = y; y0 <= y1; y0++) {
            for (int x0 = x; x0 <= x1; x0++) {
                float value = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int y_i = y0 - 1 + i;
                        int x_i = x0 - 1 + j;
                        checkCoordinate(&x_i, &y_i);
                        value += avg[i][j] * heightMap.at<unsigned char>(y_i,x_i);
                        // value += heightMap.at<unsigned char>(y_i,x_i);
                    }
                }
                value = value/9;
                heightThreshold(&value);
                smoothed.at<unsigned char>(y0,x0) = value;
            }
        }
    }

    // Old Method
    // for (int y = 0; y < this->mapHeight; y++) {
    //     for (int x = 0; x < this->mapWidth; x++) {
    //         float value = 0;
    //         for (int i = 0; i < 3; i++) {
    //             for (int j = 0; j < 3; j++) {
    //                 int y_i = y-1+i;
    //                 int x_i = x-1+j;
    //                 checkCoordinate(&x_i, &y_i);
    //                 value += avg[i][j] * heightMap.at<unsigned char>(y_i,x_i);
    //             }
    //         }
    //         value = value/9;
    //         heightThreshold(&value);
    //         smoothed.at<unsigned char>(y,x) = value;
    //     }
    // }

    cv::imwrite("heightMap.png", smoothed);
}

// void heightMap::smoothingHeightMap(IDMap idMap, particleSystem *PS) {
//     cv::Mat heightMap = cv::imread("heightMap.png", cv::IMREAD_GRAYSCALE);
//     cv::Mat smoothed  = cv::Mat::zeros(this->mapHeight, this->mapHeight, CV_8UC1);

//     std::vector <Droplet> particle =  PS->getParticleSystem(); // bisa pake auto
//     for (auto & p : particle) {
//         // starting point
//         int x = p.position.x - (int)p.radius - 2;
//         int y = p.position.y - (int)p.radius - 2;
//         // end point
//         int x1 = p.position.x + (int)p.radius + 2;
//         int y1 = p.position.y + (int)p.radius + 2;


//         for (int y0 = y; y0 <= y1; y0++) {
//             for (int x0 = x; x0 <= x1; x0++) {
//                 float value = 0;
//                 for (int i = 0; i < 3; i++) {
//                     for (int j = 0; j < 3; j++) {
//                         int y_i = y0 - 1 + i;
//                         int x_i = x0 - 1 + j;
//                         checkCoordinate(&x_i, &y_i);
//                         // value += avg[i][j] * heightMap.at<unsigned char>(y_i,x_i);
//                         value += heightMap.at<unsigned char>(y_i,x_i);
//                     }
//                 }
//                 value = value/9;
//                 if(heightThreshold(&value)) idMap.setToOne(y0, x0);
//                 smoothed.at<unsigned char>(y0,x0) = value;
//             }
//         }
//     }

//     cv::imwrite("heightMap.png", smoothed);
// }

// void heightMap::updateHeightMap(particleSystem *PS)
// {
//     int last = PS->getDrewAmmount();
//     int ammount = PS->getParticleAmmount();
//     cv::Mat heightMap = cv::imread("heightMap.png", cv::IMREAD_GRAYSCALE);

//     if (last < ammount) {
//         int dif = ammount - last;
//         std::vector <Droplet> particle =  PS->getParticleSystem(); // bisa pake auto
//         this->drawHeightMap(heightMap, particle, last, ammount);
//     }
    
//     PS->setDrewAmmount(PS->getParticleAmmount());

//     cv::imwrite("heightMap.png", heightMap);
//     // this->smoothingHeightMap(heightMap);
// }

void heightMap::checkCoordinate(int *x, int *y) {
    int dif;
    if (*x < 0) {
        dif  = -1*(*x);
        *x += 2*dif;
    }
    if (*y < 0) {
        dif = -1*(*y);
        *y += 2*dif;
    }
    if (*x > this->mapWidth - 1) {
        dif = this->mapWidth - *x;
        *x += 2*dif;
    }
    if (*y > this->mapHeight - 1) {
        dif = this->mapHeight - *y;
        *y += 2*dif;
    }
}

void heightMap::checkXY(int *x, int *y) {
    if (*x < 0) x ++;
    if (*x > this->mapWidth - 1) x--;
    if (*y < 0) y ++;
    if (*y > this->mapHeight - 1) y--;
}

// e value still can be changed
int heightMap::heightThreshold(float *value) {
    float e = 0.01;
    if (*value < e) {
        *value = 0;
        return 0;
    }
    else {
        return 1;
    }
}
