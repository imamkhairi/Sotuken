#include <heightMap.h>

heightMap::heightMap(particleSystem *ParticleSystem, int mapHeight, int mapWidth) {
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->generateHeightMap(ParticleSystem);
}

void heightMap::drawHeightMap(cv::Mat dst, std::vector <Droplet> PS, int start, int end) {
    for (int i = start; i < end; i++) {
        // starting point
        int x = PS[i].position.x - (int)PS[i].radius - 2;
        int y = PS[i].position.y - (int)PS[i].radius - 2;
        // end point
        int x1 = PS[i].position.x + (int)PS[i].radius + 2;
        int y1 = PS[i].position.y + (int)PS[i].radius + 2;

        for (int y0 = y; y0 <= y1; y0++) {
            for (int x0 = x; x0 <= x1; x0++) {
                float h = calcHeight(PS[i], x0, y0);
                if ( h > 0 ) {
                    dst.at<unsigned char>(y0, x0) = h * 10; // 50 konstansta
                }
            }
        }
    }
}

void heightMap::generateHeightMap(particleSystem *PS) {
    cv::Mat heightMap = cv::Mat::zeros(this->mapWidth, this->mapHeight, CV_8UC1);

    std::vector <Droplet> particle =  PS->getParticleSystem(); // bisa pake auto
    int start = PS->getDrewAmmount();
    int end = PS->getParticleAmmount();
    this->drawHeightMap(heightMap, particle, start, end);

    PS->setDrewAmmount(PS->getParticleAmmount());

    // this->smoothingHeightMap(heightMap);
    cv::imwrite("../Textures/heightMap.png", heightMap);
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

// PC lab smoothing = 110ms
void heightMap::smoothingHeightMap(cv::Mat heightMap) {
    cv::Mat smoothed  = cv::Mat::zeros(this->mapHeight, this->mapHeight, CV_8UC1);
    for (int y = 0; y < this->mapHeight; y++) {
        for (int x = 0; x < this->mapWidth; x++) {
            float value = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int y_i = y-1+i;
                    int x_i = x-1+j;
                    checkCoordinate(&x_i, &y_i);
                    value += avg[i][j] * heightMap.at<unsigned char>(y_i,x_i);
                }
            }
            value = value/9;
            heightThreshold(&value);
            smoothed.at<unsigned char>(y,x) = value;
        }
    }

    cv::imwrite("../Textures/heightMap.png", smoothed);
}

void heightMap::updateHeightMap(particleSystem *PS)
{
    int last = PS->getDrewAmmount();
    int ammount = PS->getParticleAmmount();
    cv::Mat heightMap = cv::imread("../Textures/heightMap.png", cv::IMREAD_GRAYSCALE);

    if (last < ammount) {
        int dif = ammount - last;
        std::vector <Droplet> particle =  PS->getParticleSystem(); // bisa pake auto
        this->drawHeightMap(heightMap, particle, last, ammount);
    }
    
    PS->setDrewAmmount(PS->getParticleAmmount());

    cv::imwrite("../Textures/heightMap.png", heightMap);
}

void heightMap::checkCoordinate(int *x, int *y) {
    if (*x < 0) *x += 1;
    if (*y < 0) *y += 1;
    if (*x > this->mapWidth - 1) *x -= 1;
    if (*y > this->mapHeight - 1) *y -= 1;
}

// e value still can be changed
void heightMap::heightThreshold(float *value) {
    float e = 0.01;
    if (*value < e) *value = 0;
    else *value = *value;
}