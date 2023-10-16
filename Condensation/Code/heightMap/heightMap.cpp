#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

int grid[10][10];

const float density = 1;
glm::vec2 p(50, 20); 

float calcRadius(float mass) {
    float r = std::cbrt((double)(3*mass)/(2*density*M_1_PI));
    return r;
}

float calcHeight(glm::vec2 p, float r, int x_i, int y_i) {
    glm::vec2 dp(p.x - x_i, p.y - y_i);
    float v = r*r - glm::length(dp);
    float h;
    if (v > 0) {
        h = std::sqrt(v);
        return h;
    } else return 0;
}

int main() {
    float r = calcRadius(5);

    cv::Mat heightMap = cv::Mat::zeros(256, 256, CV_8UC1);

    unsigned int max_y = heightMap.size().height;
    unsigned int max_x = heightMap.size().width;

    for (unsigned int y = 0; y < max_y; y++) {
        for (unsigned int x = 0; x < max_x; x++) {
            float h = calcHeight(p, r, x, y);
            if ( h > 0 ) {
                std::cout << h << std::endl;
                heightMap.at<unsigned char>(y, x) = (h/r)*255;

            }
        }
    }

    cv::imwrite("test.jpeg", heightMap);

    return 0;
}