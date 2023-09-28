#include <opencv2/opencv.hpp>
using namespace cv;

int main(void)
{
    Mat3b img = imread("meme.jpg");

    imshow("Original", img);
    waitKey();

    Rect r(100,100,200,200);
    Mat3b roi3b(img(r));

    GaussianBlur(roi3b, roi3b, Size(), 10);

    imshow("After Blur", img);
    waitKey();

    // Mat1b roiGray;
    // cvtColor(roi3b, roiGray, COLOR_BGR2GRAY);

    // threshold(roiGray, roiGray, 200, 255, THRESH_BINARY);

    // Mat3b roiGray3b;
    // cvtColor(roiGray, roiGray3b, COLOR_GRAY2BGR);
    // roiGray3b.copyTo(roi3b);

    // imshow("After Threshold", img);
    // waitKey();

    return 0;
}