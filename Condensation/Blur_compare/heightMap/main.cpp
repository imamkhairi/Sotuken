#include <iostream>
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 

int main() 
{
    std::string path = "Lena.png";
    cv::Mat image = cv::imread(path);
    // cv::Mat blurImage = cv::Mat(cv::Size(100, 100), CV_8UC1);
    cv::Mat blurImage;

    cv::GaussianBlur(image, blurImage, cv::Size(7, 7), 5, 0);

    cv::imshow("Image", image);
    cv::imshow("blurred", blurImage);
    
    cv::waitKey(0);
    return (0);
}