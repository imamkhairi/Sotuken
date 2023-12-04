#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

#define IMAGE_HEIGHT 300
#define IMAGE_WIDTH 300


std::vector<glm::vec2> metaballs;

void addMetaball(float x, float y, std::vector<glm::vec2> *metaballs){
    metaballs->push_back(glm::vec2(x, y));
}

float Distance(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x1-x2, 2) + std::pow(y1-y2,2));
}

void checkSum(float sum) {
    if (sum < 0) std::cout << "kurang 0" << std::endl;
    else if(sum > 255) std::cout << "kelebihan dr 255" << std::endl;
}

int main() {
    cv::Mat image = cv::Mat::zeros(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC1);
    
    //// Add metaball
    addMetaball(50.0f, 50.0f, &metaballs);
    addMetaball(150.0f, 50.0f, &metaballs);
    // addMetaball(75.0f, 100.0f, &metaballs);


    //// Loop all the pixels
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            float d = 0, sum = 0;
            
            // float h = 100;
            // float r = 1000;

            float R = 50;
            for (auto & m :metaballs) {
                d = Distance(m.x, m.y, x, y);
                // sum += 100.0f * std::exp(-1.f * 0.00003 * d * d);
                // sum += 100.0f * std::exp(-1.f * 0.0002 * d * d);
                
                //// Sensei
                // sum += 1/(d*d+1.0);

                ///// Blob
                // float b = -2.3; // blobbiness
                // float r = 10; // radius
                // sum += r * std::exp(b/(r*r)*d*d-b);


                //// Test (new)
                // sum += h * std::exp(-1 * 1/r*d*d);

                //// paper meijo daigaku
                sum += 1 / (1 + std::pow((d/R), 2.8));;
            }
            // sum -= h/2;
            sum -= 0.5;


            // if(sum > 50) std::cout << sum << std::endl;
            
            // if (sum >= 50) sum = 255;   
            // else if (sum >= 40) sum = 126;
            // else sum = 0;   
            // if (sum < 20) sum = 0;
            // if(sum > 50)
            if(sum > 0) sum =  sum * 100;
            // if (sum > 255) sum = 255;
            image.at<unsigned char>(y, x) = sum;
            
            }
    }
    
    cv::imwrite("../test.png", image);

    metaballs.clear();

    return 0;
}

