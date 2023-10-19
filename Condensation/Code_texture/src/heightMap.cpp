#include <heightMap.h>

heightMap::heightMap(std::vector <Droplet> ParticleSystem, int mapHeight, int mapWidth) {
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->generateHeightMap(ParticleSystem);
}

void heightMap::generateHeightMap(std::vector <Droplet> particleSystem) {
    srand(time(0));

    cv::Mat heightMap = cv::Mat::zeros(this->mapWidth, this->mapHeight, CV_8UC1);
    for (unsigned int y = 0; y < this->mapHeight; y++) {
        for (unsigned int x = 0; x < this->mapWidth; x++) {
            // heightMap.at<unsigned char>(y, x) = h*(255/r); // 255/r ini asumsi max 255
            for (auto & particle : particleSystem) {
                float h = calcHeight(particle, x, y);
                if ( h > 0 ) {
                    heightMap.at<unsigned char>(y, x) = h*50; // 50 konstansta
                }
            }
        }
    }

    this->smoothingHeightMap(heightMap);
}

float heightMap::calcHeight(Droplet a, int x_i, int y_i) {
    glm::vec2 dp(a.position.x - x_i, a.position.y - y_i);
    float value = a.radius*a.radius - glm::length(dp);
    float height;
    if (value > 0) {
        height = std::sqrt(value);
        return height;
    } else return 0;
}

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
            if (value < 1 && value > 0)  std::cout << value << std::endl;
            heightThreshold(&value);
            smoothed.at<unsigned char>(y,x) = value;
        }
    }

    cv::imwrite("../Textures/heightMap.jpg", smoothed);
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

// int main() {

//     

//     // calculate height
//     generateHeightMap(particleSystem, &heightMap);

//     // smoothing 
//     smoothingHeightMap(heightMap, &smoothed);

//     cv::imwrite("height.jpg", heightMap);
//     cv::imwrite("smoothed.jpg", smoothed);

//     return 0;
// }