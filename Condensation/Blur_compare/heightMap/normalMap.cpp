#include <iostream>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// dibagian bawah muncul garis putih

const int height = 1024;
const int width = 1024;

float z = -1;

int sobel_x[3][3] = {
    1, 0, -1,
    2, 0, -2,
    1, 0, -1
};

int sobel_y[3][3] = {
    1,  2,  1,
    0,  0,  0,
    -1, -2, -1
};


void checkCoordinate(int *x, int *y) {
    if (*x < 0) *x += 1;
    if (*y < 0) *y += 1;
    if (*x > width - 1) *x -= 1;
    if (*y > height - 1) *y -= 1;
}

void offsetAndClamp(int *value) {
    *value += 128;
    if (*value < 0) *value = 0;
    else if(*value > 255) *value = 255;
}

// camera ke z-

void calcGradient(cv::Mat heightMap, cv::Mat *grad_x, cv::Mat *grad_y) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int value_x = 0;
            int value_y = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int y_i = y-1+i;
                    int x_i = x-1+j;
                    checkCoordinate(&x_i, &y_i);
                    value_x += sobel_x[i][j] * heightMap.at<unsigned char>(y_i,x_i);
                    value_y += sobel_y[i][j] * heightMap.at<unsigned char>(y_i, x_i);
                }
            }
            offsetAndClamp(&value_x);
            offsetAndClamp(&value_y);
            grad_x->at<unsigned char>(y,x) = value_x;
            grad_y->at<unsigned char>(y,x) = value_y;
        }
    }
}

void generateNormalMap(cv::Mat *normalMap, cv::Mat grad_x, cv::Mat grad_y) {
    for (int y = 0; y < height; y++) {
    int i = 0;
    int j = 0;
        for (int x = 0; x < width * 3 ; x +=3) {
            normalMap->at<unsigned char>(y,x) = 255;
            // std::cout << x << std::endl;
            normalMap->at<unsigned char>(y,x+1) = grad_y.at<unsigned char>(y, i++);
            normalMap->at<unsigned char>(y,x+2) = grad_x.at<unsigned char>(y, j++);
        }
    }
}

int main() {
    cv::Mat heightMap = cv::imread("heightMap.png", cv::IMREAD_GRAYSCALE);
    cv::Mat normalMap = cv::Mat::zeros(width, height, CV_8UC3);
    cv::Mat grad_x = cv::Mat::zeros(width, height, CV_8UC1);
    cv::Mat grad_y = cv::Mat::zeros(width, height, CV_8UC1);

    calcGradient(heightMap, &grad_x, &grad_y);

    generateNormalMap(&normalMap, grad_x, grad_y);

    // cv::imwrite("grad_x.jpeg", grad_x);
    // cv::imwrite("grad_y.jpeg", grad_y);
    cv::imwrite("normal.png", normalMap);
    return 0;
}