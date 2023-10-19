#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

const int height = 256;
const int width = 256;

const int avg[3][3] = {
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
};

struct droplet
{
    glm::vec3 position;
    float mass;
    float radius;
    const float density = 1;
};

float calcRadius(droplet a) {
    float r = std::cbrt((double)(3*a.mass)/(2*a.density*M_1_PI));
    // std::cout << "func : " << a.radius << std::endl;
    return r;
}

float calcHeight(droplet a, int x_i, int y_i) {
    glm::vec2 dp(a.position.x - x_i, a.position.y - y_i);
    float value = a.radius*a.radius - glm::length(dp);
    float height;
    if (value > 0) {
        height = std::sqrt(value);
        return height;
    } else return 0;
}

void initiateDroplet(droplet *a) {
    a->mass = 1.3f;
    a->radius = calcRadius(*a);
    a->position = glm::vec3((rand() % (int)(width - 2*(a->radius + 2))) + (int)(a->radius + 2), (rand() % (int)(height - 2*(a->radius + 2))) + (int)(a->radius + 2), 10); 
    // z coordinate ini akan diubah
}

void checkCoordinate(int *x, int *y) {
    if (*x < 0) *x += 1;
    if (*y < 0) *y += 1;
    if (*x > width - 1) *x -= 1;
    if (*y > height - 1) *y -= 1;
}

void heightThreshold(float *value) {
    float e = 0.01;
    if (*value < e) *value = 0;
    else *value = *value;
}

void initiateParticleSystem(std::vector <droplet> *particleSystem) {
    int particleAmmount = 30;
    for (int i = 0; i < particleAmmount; i++) {
        droplet a;
        initiateDroplet(&a);
        particleSystem->push_back(a);
    }
}

void generateHeightMap(std::vector <droplet> particleSystem, cv::Mat *heightMap) {
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // heightMap.at<unsigned char>(y, x) = h*(255/r); // 255/r ini asumsi max 255
            for (auto & particle : particleSystem) {
                float h = calcHeight(particle, x, y);
                if ( h > 0 ) {
                    heightMap->at<unsigned char>(y, x) = h*50; // 50 konstansta
                }
            }
        }
    }
}

void smoothingHeightMap(cv::Mat heightMap, cv::Mat *smoothed) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
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
            smoothed->at<unsigned char>(y,x) = value;
        }
    }
}


int main() {
    srand(time(0));

    cv::Mat smoothed  = cv::Mat::zeros(width, height, CV_8UC1);
    cv::Mat heightMap = cv::Mat::zeros(width, height, CV_8UC1);

    // initiate particle system
    std::vector <droplet> particleSystem;
    initiateParticleSystem(&particleSystem);

    // calculate height
    generateHeightMap(particleSystem, &heightMap);

    // smoothing 
    smoothingHeightMap(heightMap, &smoothed);

    cv::imwrite("height.jpg", heightMap);
    cv::imwrite("smoothed.jpg", smoothed);

    return 0;
}