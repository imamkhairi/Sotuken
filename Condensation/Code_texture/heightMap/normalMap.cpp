#include <iostream>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// dibagian bawah muncul garis putih

const int height = 256;
const int width = 256;

void checkCoordinate(int *x, int *y) {
    if (*x < 0) *x += 1;
    if (*y < 0) *y += 1;
    if (*x > width) *x -= 1;
    if (*y > height) *y -= 1;
}

void addOffset(int *value) {
    *value += 128;
    if (*value < 0) *value = 0;
    else if(*value > 255) *value = 255;
}

// camera ke z-

int main() {
    float z = -1;

    int sobel_x[3][3] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    
    int sobel_y[3][3] = {
         1,  2,  1,
         0,  0,  0,
        -1, -2, -1
    };

    cv::Mat heightMap = cv::imread("test.jpeg", cv::IMREAD_GRAYSCALE);
    
    // cv::Mat normalMap = cv::Mat::zeros(3, 3, CV_8UC3);
    // for (int y = 0; y < height; y++) {
    //     for (int x = 0; x < width * 3; x) {
    //         normalMap.at<unsigned char>(y,x++) = 100;
    //         normalMap.at<unsigned char>(y,x++) = 255;
    //         normalMap.at<unsigned char>(y,x++) = 0;
    //     }
    // }

    cv::Mat test = cv::Mat::zeros(width, height, CV_8UC1);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int value = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int x_i = x-1+i;
                    int y_i = y-1+j;
                    checkCoordinate(&x_i, &y_i);
                    value += sobel_y[i][j] * heightMap.at<unsigned char>(y_i,x_i);
                }
            }
            addOffset(&value);
            test.at<unsigned char>(y,x) = value;
        }
    }

    cv::imwrite("normal.jpeg", test);
    return 0;
}