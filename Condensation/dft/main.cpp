#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Missing required argument" << std::endl;
        return -1;
    }

    std::string fileName = argv[1];
    int filterOption = std::stoi(argv[2]);

    cv::Mat img = cv::imread(fileName, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Error: Could not read the image file." << std::endl;
        return -1;
    }

    cv::Mat padded;
    int m = cv::getOptimalDFTSize(img.rows);
    int n = cv::getOptimalDFTSize(img.cols);
    cv::copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size().height, padded.size().width, CV_32F)};
    std::cout << "test" << std::endl;
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);

    cv::dft(complexImg, complexImg);

    int centerX = padded.cols / 2;
    int centerY = padded.rows / 2;

    cv::Mat mask = cv::Mat::zeros(padded.size().height, padded.size().width, CV_32FC2);
    cv::Rect roi(centerX - filterOption, centerY - filterOption, 2 * filterOption, 2 * filterOption);
    mask(roi) = 1;

    cv::Mat filteredImg;
    cv::multiply(complexImg, mask, filteredImg);

    cv::idft(filteredImg, filteredImg);
    cv::split(filteredImg, planes);
    cv::magnitude(planes[0], planes[1], planes[0]);
    cv::Mat magnitudeImage = planes[0];

    magnitudeImage += cv::Scalar::all(1);
    cv::log(magnitudeImage, magnitudeImage);
    magnitudeImage = magnitudeImage(cv::Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));

    cv::normalize(magnitudeImage, magnitudeImage, 0, 1, cv::NORM_MINMAX);

    cv::imshow("Filtered Image", magnitudeImage);
    cv::waitKey(0);

    return 0;
}
