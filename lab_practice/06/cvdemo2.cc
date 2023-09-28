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


// デプス画像をカラー画像へサーモグラフィー的に変換
cv::Mat GlViewColor(cv::Mat depth)
{
	static cv::Mat	image(480, 640, CV_8UC3);	// 変換後のカラー画像データの容器
			// サイズ 640x480 は Kinect の RGBDカメラと同じに
	uchar *depth_mid = (uchar*)(image.data);
	int i;
	for (i = 0; i < 640*480; i++) {
		int lb = ((ushort *)depth.data)[i] % 256;	// 下位8bit（lower byte）
		int ub = ((ushort *)depth.data)[i] / 256;	// 上位8bit（upper byte）
					// 元々のデプス値は ushort型 16bit
		switch (ub) {	// デプスの上位の値に応じて大体の色を決めるよ
		case 0:
			depth_mid[3*i+2] = 255;			// R
			depth_mid[3*i+1] = 255-lb;		// G
			depth_mid[3*i+0] = 255-lb;		// B
				// 下位の値によっても微妙に変化させるよ．
				// lb が 0 => 255 のとき，white(R+G+B) => red(R)
			break;
		case 1: //9
			depth_mid[3*i+2] = 255;
			depth_mid[3*i+1] = lb;
			depth_mid[3*i+0] = 0;
						// red(R) => yellow(R+G)
			break;
		case 2:
			depth_mid[3*i+2] = 255-lb;
			depth_mid[3*i+1] = 255;
			depth_mid[3*i+0] = 0;
						// yellow(R+G) => green(G)
			break;
		case 3:
			depth_mid[3*i+2] = 190;
			depth_mid[3*i+1] = 255;
			depth_mid[3*i+0] = lb;
						// green(G) => cyan(G+B)
			break;
		case 4:
			depth_mid[3*i+2] = 0;
			depth_mid[3*i+1] = 255-lb;
			depth_mid[3*i+0] = 255;
						// cyan(G+B) => blue(B)
			break;
		case 5: //13
			depth_mid[3*i+2] = 0;
			depth_mid[3*i+1] = 0;
			depth_mid[3*i+0] = 255-lb;
						// blue(B) => black(0)
			break;
		default:
			depth_mid[3*i+2] = 0;
			depth_mid[3*i+1] = 0;
			depth_mid[3*i+0] = 0;
						// black
			break;
		}
	}
	return image;
}

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

// ini yang gagal
// void removeBG(cv::Mat &depth) {
// 	ushort *q = (ushort *)depth.data;
	
// 	int n = depth.rows * depth.cols;
// 	// std::cout << n << std::endl;
// 	for (int i = 0; i < n; i ++) {
// 		if(!*q > 0) std::cout << *q << std::endl;
// 	}
// }

// Kinect からカラー画像を取得する関数
// freenect_sync_get_video_cv() の C++ API版クローン
cv::Mat freenect_sync_get_rgb_cv2(int index)
{
	static cv::Mat	image(480, 640, CV_8UC3);
	static uchar	*data = NULL;
	unsigned int	ts;
	if (freenect_sync_get_video((void**)&data, &ts, index, FREENECT_VIDEO_RGB))
			//					video, timestamp, index, fmt
			// anggap untuk load data video dulu aja
		data = NULL;
	image.data = data;
	return (image);
}

// using namespace std;
// int main(int argc, char **argv)
// {
// 	cv::Mat	image;
// 	cv::Mat	depth;
       
// 	while (cv::waitKey(10) < 0) {	// キー入力で終了
// 		image = freenect_sync_get_rgb_cv2(0);	// カラー画像を取得
// 			// get color image data

// 		if (!image.data) {
// 			printf("Error: image: Kinect not connected?\n");
// 			return (-1);
// 		}
// 		cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
// 			// カラーチャネル順序の変換（Kinect：RGB → OpenCV：BGR）
// 			// karena input dari kinect itu RGB harus diganti ke BGR dulu kalau mau masuk opencv

// 		// デプス画像を取得
// 		// depth = freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_11BIT);
// 				// 生データ版（D値は距離そのものではない）
// 		// depth = freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_MM);
// 				// 補正版1（D値をmm単位の距離に変換）
// 		depth = freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_REGISTERED);
// 				// 補正版2（RGB画像に位置合わせ，通常はコレでOK？）

// 		// std::cout << depth << std::endl;

// 		if (!depth.data) {
// 			printf("Error: depth: Kinect not connected?\n");
// 			return (-1);
// 		}
// 		removeBG(depth, image);

// 		cv::imshow("BG Removed", image);	// カラー画像を表示
// 			/* cv::namedWindow("RGB") とか省略してますが大丈夫．
// 				ウィンドウは自動生成されます． */

// 		// cv::imshow("Depth", depth);	// デプス画像をそのまま表示
// 			/* ↑これも実行可能ですが...画素値16bitの内，
// 				深度値としては11bitしか使わないので，
// 				そのまま表示しても暗すぎて，真っ黒な画像にしか見えない．
// 				というわけで... ↓*/
			
// 		// cv::imshow("Depth", GlViewColor(depth)); // yang warna2 aneh itu di sini
// 			// デプス画像をカラー化して表示
// 			/* その他の表示方法としては，
// 				画素値（深度値）を32倍（5bitシフト）して
// 				明るいモノクロ画像に変換してみては？ */

// 		cv::imshow("GrayScale", depth*32);	// グレースケール表示
// 		// removeBG(depth);

// 	}
// 	return (0);
// }

#define getDepth()	freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_REGISTERED)
#define getColor()	freenect_sync_get_rgb_cv2(0)

void fatal(const char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char **argv) {

	cv::namedWindow("BG Removed", cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("GrayScale", cv::WINDOW_GUI_NORMAL);
	cv::resizeWindow("BG Removed", 640, 480);
	cv::resizeWindow("GrayScale", 640, 480);
	cv::moveWindow("BG Removed", 100, 0);
	cv::moveWindow("BG Removed", 800, 0);

	while(1) {
		if (cv::getWindowProperty("RGB", cv::WND_PROP_VISIBLE) < 1.0) break;
		if (cv::getWindowProperty("RGB", cv::WND_PROP_VISIBLE) < 1.0) break;

		cv::Mat color = getColor();
		if(!color.data) fatal("Capture error");

		cv::Mat depth = getDepth();
		if(!depth.data) fatal("Depth error");

		cv::cvtColor(color, color, cv::COLOR_RGB2BGR);
		cv::imshow("BG Removed", color);

		depth.convertTo(depth, -1, 32, 0);
		cv::imshow("GrayScale", depth);

		int key = cv::waitKey(10);
		if (key == 'q') break;
		if (key == '\e') break;
	}

	freenect_sync_stop();

	cv::destroyWindow("GrayScale");
	cv::destroyWindow("BG Removed");


	return 0;
}