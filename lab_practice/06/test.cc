/*
freenect-glview の OpenCV C API版クローン freenect-cvdemo
https://github.com/OpenKinect/libfreenect/blob/master/wrappers/opencv
これはそれ（cvdemo.cpp + libfreenect_cv.cpp）を C++ API化したもの
$ c++ cvdemo2.cc `pkg-config --cflags --libs libfreenect opencv` -lfreenect_sync -o cvdemo2
*/
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "libfreenect.h"
#include "libfreenect_sync.h"

// versi abal abal
void removeBG(cv::Mat &depth, cv::Mat &image) {
	cv::Vec3b *p = (cv::Vec3b *)(image.data);
	
	int i;

	for ( int i = 0; i < 640*480; i++) {
		int lb = ((ushort *)depth.data)[i] % 256;	// 下位8bit（lower byte）
		int ub = ((ushort *)depth.data)[i] / 256;	// 上位8bit（upper byte）

		// if(ub == 2) {
		// 		*p = (cv::Vec3b)((cv::Vec3b)(*p));
		// } else {
		// 	(*p)[3*i+2] = 0;			// R
		// 	(*p)[3*i+1] = 0;			// G
		// 	(*p)[3*i+0] = 0;			// B
		// }
		switch(ub) {
			// case 0:
			// case 1:
			case 2:
				*p = (cv::Vec3b)((cv::Vec3b)(*p));
				break;
			case 3:
				*p = (cv::Vec3b)((cv::Vec3b)(*p));
				break;
			// case 4:
			// 	*p = (cv::Vec3b)((cv::Vec3b)(*p));
			// 	break;
			// case 5:
			default:
				(*p)[3*i+2] = 40;			// R
				(*p)[3*i+1] = 40;			// R
				(*p)[3*i+0] = 40;			// R
				break;
		}
	}	
}


// ini yang gagal
// void removeBG(cv::Mat &depth) {
// 	ushort *q = (ushort *)depth.data;
	
// 	int n = depth.rows * depth.cols;
// 	// std::cout << n << std::endl;
// 	for (int i = 0; i < n; i ++) {
// 		if(!*q > 0) std::cout << *q << std::endl;
// 	}
// }

// Kinect からデプス画像を取得する関数
// freenect_sync_get_depth_cv() の C++ API版クローン＋補正機能
/* 補正方式 fmt:
	FREENECT_DEPTH_11BIT		生データ版（D値は距離そのものではない）
	FREENECT_DEPTH_MM		補正版1（D値をmm単位の距離に変換）
	FREENECT_DEPTH_REGISTERED	補正版2（RGB画像に位置合わせ）

	FREENECT_DEPTH_11BIT: 11 bit depth information in one uint16_t/pixel 
	FREENECT_DEPTH_MM: depth to each pixel in mm, but left unaligned to RGB image
	FREENECT_DEPTH_REGISTERED: processed depth data in mm, aligned to 640x480 RGB 
*/
cv::Mat freenect_sync_get_depth_cv2(int index, freenect_depth_format fmt)
{
	static cv::Mat	image(480, 640, CV_16UC1);
	static uchar	*data = NULL;
	unsigned int	ts;
	if (freenect_sync_get_depth((void**)&data, &ts, index, fmt))
		// 						depthm, timestamp, index, fmt
		data = NULL;
	image.data = data;
	return (image);
}

// Kinect からカラー画像を取得する関数
// freenect_sync_get_video_cv() の C++ API版クローン
cv::Mat freenect_sync_get_rgb_cv2(int index)
{
	static cv::Mat	image(480, 640, CV_8UC3);
	static uchar	*data = NULL;
	unsigned int	ts;
	if (freenect_sync_get_video((void**)&data, &ts, index, FREENECT_VIDEO_RGB))
		data = NULL;
	image.data = data;
	return (image);
}

#define getDepth()	freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_REGISTERED)
#define getColor()	freenect_sync_get_rgb_cv2(0)

void fatal(const char *msg) {
	perror(msg);
	exit(1);
}

void removeBG(cv::Mat color, cv::Mat depth, cv::Mat &fg, ushort td) {
    if(fg.data != color.data) fg = cv::Mat(color.size(), CV_8UC3);

    for (int y = 0; y < color.rows; y++) 
    for (int x = 0; x < color.cols; x++) {
        ushort d = depth.at<ushort>(y, x);
        if (d == 0) {
            fg.at<cv::Vec3b>(y,x) = cv::Vec3b(0x7F, 0x7F, 0x7F);
        } else if (d > td){
            fg.at<cv::Vec3b>(y,x) = cv::Vec3b(0x7F, 0x7F, 0x7F);
        } else {
            fg.at<cv::Vec3b>(y,x) = color.at<cv::Vec3b>(y,x);
        }
    }
}

void removeBG2(cv::Mat color, cv::Mat depth, cv::Mat &fg, ushort td) {
    if(fg.data != color.data) fg = cv::Mat(color.size(), CV_8UC3);

    cv::Vec3b *f = (cv::Vec3b *)(fg.data);
    cv::Vec3b *c = (cv::Vec3b *)(color.data);
    ushort    *d = (ushort *)(depth.data);

    cv::Vec3b gray = cv::Vec3b(0x7F, 0x7F, 0x7F);
}

int main(int argc, char **argv) {
	cv::namedWindow("Color", cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("GrayScale", cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("BG Remove", cv::WINDOW_GUI_NORMAL);
	cv::resizeWindow("Color", 640, 480);
	cv::resizeWindow("GrayScale", 640, 480);
	cv::resizeWindow("BG Remove", 640, 480);
	cv::moveWindow("Color", 100, 0);
	cv::moveWindow("GrayScale", 800, 0);
	cv::moveWindow("BG Remove", 100, 500);

	while(1) {
        ushort td = 2000;

		if (cv::getWindowProperty("GrayScale", cv::WND_PROP_VISIBLE) < 1.0) break;
		if (cv::getWindowProperty("Color", cv::WND_PROP_VISIBLE) < 1.0) break;
		if (cv::getWindowProperty("BG Remove", cv::WND_PROP_VISIBLE) < 1.0) break;

		cv::Mat color = getColor();
		if(!color.data) fatal("color error");
		cv::cvtColor(color, color, cv::COLOR_RGB2BGR);

		cv::Mat depth = getDepth();
		if(!depth.data) fatal("Depth error");

        cv::Mat fg;
        removeBG(color, depth, fg, td);


        cv::imshow("BG Remove", fg);
		cv::imshow("Color", color);
		depth.convertTo(depth, -1, 32, 0);
		cv::imshow("GrayScale", depth);

		int key = cv::waitKey(10);
		if (key == 'q') break;
		if (key == '\e') break;
	}

	freenect_sync_stop();

	cv::destroyWindow("GrayScale");
	cv::destroyWindow("BG Remove");
	cv::destroyWindow("Color");

	return 0;
}