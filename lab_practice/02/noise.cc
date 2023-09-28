/*
OpenCVのサンプルプログラム noise.cc：ノイズ付与の例

コンパイル：$ c++ noise.cc `pkg-config --cflags --libs opencv` -o noise
実行：$ ./noise [カメラ番号] &
*/
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

// using namespace cv;

// 強制終了関数
void fatal(const char *msg)
{
	perror(msg);
	exit(1);
}
 
// ノイズ付与フィルタ
// src：入力画像
// dst：出力画像
// n：ノイズ量
int noise(const cv::Mat& src, cv::Mat& dst, int n, int noisetype)
{
	int	w, h, ws; // widht, height, horizontal count
	int	x, y; // x and y coordinate
	
	int	i, c;
	// i for index
	// c for colour?
	
	unsigned char	*p;
	unsigned char v;

	// unsigned char	v = 0;		// ゴマ塩ノイズ付与の場合の画素値
	// some random pixel at (x,y) will be 0

	// unsigned char	v = 255;	// クリスマスノイズ付与の場合の画素値
	// for a random coordinate (x,y) give that 255 value
	// which means B G R will be full color for that pixel
	// that is why its called christmas noise

	v = (noisetype == 0) ? 0 : 255;

	// 入力画像の検査
	if (src.empty()) return (1);		// 入力なしの場合
	if (src.type() != CV_8UC3) return (1);	// 8bit×3ch 画像じゃない場合
	// 8bit unsigned char 3 channel

	// 変数の準備
	w = src.cols;		// collumn numbers
	h = src.rows;		// row numbers
	ws = src.step;		// 画素値配列１行分のデータサイズ
	std::cout << w << std::endl;
	// std::cout << src.step << std::endl; 1920 -> 640 * 3 (bgr)


	// 出力画像の準備
	if (dst.data != src.data) src.copyTo(dst);
		// 入出力の画像データの実体が異なる場合だけ，入力を出力へコピー．
		// これで入出力が同じ場合 noise(src, src, ...) でも無問題．
	p = (unsigned char *)dst.data;	// 画素値配列の先頭アドレス

	// ノイズ付与
	for (i = 0; i < n; i++) {
		// 画素・成分をランダムに選択
		x = rand()%w;
		y = rand()%h;
		c = rand()%3;

		// 画素(x,y) の成分c を変更
		*(p + ws*y + x*3 + c) = v;
		// x*3 because there is B G R for one x coordinate
	}
	return (0);
}
 
int main(int argc, char *argv[])
{
	const char	*window = "noise";
	int	w = 640;
	int	h = 480;
	int	n = 250000;	// ノイズ量]
	int noisetype = 0;

	int	dev = 0;
	if (argc > 1) dev = *argv[1] - '0';

	cv::VideoCapture	capture;
	if (!capture.open(dev)) fatal("キャプチャ開始に失敗");
	capture.set(cv::CAP_PROP_FRAME_WIDTH, w);
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, h);

	cv::namedWindow(window, cv::WINDOW_NORMAL);
	cv::resizeWindow(window, w, h);

	while (cv::getWindowProperty(window, cv::WND_PROP_VISIBLE) > 0.0) {
		cv::Mat	src;			// 入力画像
		if (!capture.read(src)) fatal("キャプチャに失敗");

		cv::Mat	dst;			// 出力画像
		if (noise(src, dst, n, noisetype) == 1) break;	// ノイズ付与

		cv::imshow(window, dst);

		int key = cv::waitKey(10);
		if (key == 'q') break;

		switch(key) {
			case '.' :
				n = (n + 10000 < 640*480) ? n += 10000 : 640*480;
				break;
			case ',' :
				n = (n - 10000 > 0) ? n -= 10000 : 0;
				// n = 200;
				break;
			case ' ':
				noisetype = (noisetype +1)%2;
		}
	}
	if (capture.isOpened()) capture.release();
	cv::destroyWindow(window);

	return (0);
}

/* 練習問題：
(1) キーボード操作によるノイズ量の増減を可能にせよ． done 
(2) キーボード操作によるノイズの種類の変更を可能にせよ． done
*/

