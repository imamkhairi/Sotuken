#include <iostream>
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 
#include <time.h>

int main() 
{
    std::string path = "heightMap.png";
    // std::string path = "Lena.png";
    cv::Mat image = cv::imread(path);
    // cv::Mat blurImage = cv::Mat(cv::Size(100, 100), CV_8UC1);
    cv::Mat blurImageGaussian;
    cv::Mat blurImageBox;

    clock_t tStart = clock();
    cv::GaussianBlur(image, blurImageGaussian, cv::Size(3, 3), 5, 0);
    printf("Gaussian blur Height Map: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));
    tStart = clock();
    cv::blur(image, blurImageBox, cv::Size(3, 3));
    cv::threshold(blurImageBox, blurImageBox, 0.05, 255, cv::THRESH_TOZERO);
    printf("box Height Map: %.5f ms\n", (double)(clock() - tStart)/(CLOCKS_PER_SEC/1000));

    // cv::imshow("Image", image);
    // cv::imshow("gaussian", blurImageGaussian);
    // cv::imshow("box", blurImageBox);

    cv::imwrite("blur_gaussian.png", blurImageGaussian);
    cv::imwrite("blur_box.png", blurImageBox);
    
    cv::waitKey(0);
    return (0);
}