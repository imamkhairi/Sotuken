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

struct droplet
{
    glm::vec2 position;
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
    a->position = glm::vec2((rand() % width) + 1, (rand() % height) + 1);
    a->mass = 1.5f;
    a->radius = calcRadius(*a);
}

int main() {
    cv::Mat heightMap = cv::Mat::zeros(256, 256, CV_8UC1);
    unsigned int max_y = heightMap.size().height;
    unsigned int max_x = heightMap.size().width;

    srand(time(0));
    std::vector <droplet> dropletParticles;
    int particleAmmount = 50;
    for (int i = 0; i < particleAmmount; i++) {
        droplet a;
        initiateDroplet(&a);
        dropletParticles.push_back(a);
    }

    for (unsigned int y = 0; y < max_y; y++) {
        for (unsigned int x = 0; x < max_x; x++) {
            float h = calcHeight(dropletParticles[0], x, y);
            // heightMap.at<unsigned char>(y, x) = h*(255/r); // 255/r ini asumsi max 255
            for (auto & particle : dropletParticles) {
                float h = calcHeight(particle, x, y);
                if ( h > 0 ) {
                    heightMap.at<unsigned char>(y, x) = h*50; // 50 konstansta
                }
            }
        }
    }

    cv::imwrite("test.jpeg", heightMap);

    return 0;
}