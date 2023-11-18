#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

#define IMAGE_HEIGHT 600
#define IMAGE_WIDTH 600


std::vector<glm::vec2> metaballs;

void addMetaball(float x, float y, std::vector<glm::vec2> *metaballs){
    metaballs->push_back(glm::vec2(x, y));
}

float Distance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x1-x2, 2) + std::pow(y1-y2,2));
}

int main() {
    cv::Mat image = cv::Mat::zeros(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC1);
    
    //// Add metaball
    addMetaball(50.0f, 50.0f, &metaballs);
    addMetaball(100.0f, 100.0f, &metaballs);
    addMetaball(50.0f, 100.0f, &metaballs);


    //// Loop all the pixels
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            float d = 0, sum = 0;
            
            for (auto & m :metaballs) {
                d = Distance(m.x, m.y, x, y);
                sum += 100.0f * std::exp(-1.f * 0.0008 * d * d);
            }
            
            // if (sum >= 50) sum = 255;   
            // else if (sum >= 40) sum = 255;
            // else sum = 0;   
            // if (sum < 20) sum = 0;
            image.at<unsigned char>(y, x) = sum;
        }
    }
    
    cv::imwrite("../test.png", image);

    metaballs.clear();

    return 0;
}

