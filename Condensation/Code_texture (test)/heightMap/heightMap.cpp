#include <heightMap.h>

heightMap::heightMap(std::vector <Droplet> ParticleSystem, int mapHeight, int mapWidth) {
    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;
    this->generateHeightMap(ParticleSystem);
}

// need optimization
void heightMap::generateHeightMap(std::vector <Droplet> particleSystem) {
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

    for (auto & particle : particleSystem) {
        // starting point
        int x = particle.position.x - (int)particle.radius - 2;
        int y = particle.position.y - (int)particle.radius - 2;
        // end point
        int x1 = particle.position.x + (int)particle.radius + 2;
        int y1 = particle.position.y + (int)particle.radius + 2;

        // std::cout << "center :" << particle.position.x << ", " << particle.position.y << std::endl;
        // std::cout << "radius :" << particle.radius << std::endl;
        // std::cout << "radius :" << (int)particle.radius << std::endl;
        // std::cout << "start :" << x << ", " << y << std::endl;
        // std::cout << "end :" << x1 << ", " << y1 << std::endl;

        for (int y0 = y; y0 <= y1; y0++) {
            for (int x0 = x; x0 <= x1; x0++) {
                float h = calcHeight(particle, x0, y0);
                if ( h > 0 ) {
                    heightMap.at<unsigned char>(y0, x0) = h * 10; // 50 konstansta
                }
            }
        }
    }

    this->smoothingHeightMap(heightMap);
    // cv::imwrite("heightMap.png", heightMap);
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
            // if (value < 1 && value > 0)  std::cout << value << std::endl;
            heightThreshold(&value);
            smoothed.at<unsigned char>(y,x) = value;
        }
    }

    cv::imwrite("heightMap.png", smoothed);
}

void heightMap::updateHeightMap()
{
    cv::Mat heightMap = cv::imread("heightMap.png", cv::IMREAD_GRAYSCALE);
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