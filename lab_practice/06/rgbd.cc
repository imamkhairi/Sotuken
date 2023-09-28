/*
Freenect x OpenCV の最小限のサンプルプログラム rgbd.c：RGBDカメラモニタ
コンパイル：$ c++ rgbd.cc `pkgconf --cflags --libs libfreenect opencv` -lfreenect_sync -o rgbd
実行：$ ./rgbd &
操作：[Q]or[Esc]で終了
*/
#include <opencv2/opencv.hpp>
#include <libfreenect.h>
#include <libfreenect_sync.h>
#include <stdlib.h>
#include <stdio.h>

// using namespace	cv;

// Kinect からデプス画像を取得する関数
// freenect_sync_get_depth_cv() の C++ API版クローン＋補正機能
/* 補正方式 fmt:
	FREENECT_DEPTH_11BIT		生データ版（D値は距離そのものではない）
	FREENECT_DEPTH_MM		補正版1（D値をmm単位の距離に変換）
	FREENECT_DEPTH_REGISTERED	補正版2（RGB画像に位置合わせ）
*/
cv::Mat freenect_sync_get_depth_cv2(int index, freenect_depth_format fmt)
{
	static cv::Mat	image(480, 640, CV_16UC1);
	static uchar	*data = NULL;
	unsigned int	ts;
	if (freenect_sync_get_depth((void**)&data, &ts, index, fmt))
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

// ↑関数名が長たらしいので...短縮名をマクロ定義してしまおう↓
#define getDepth()	freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_REGISTERED)
#define getColor()	freenect_sync_get_rgb_cv2(0)

// 強制終了関数
void fatal(const char *msg)
{
	perror(msg);		// エラーメッセージを表示
	exit(1);
}

int main(int argc, char **argv)
{
	// 準備
	cv::namedWindow("RGB", cv::WINDOW_GUI_NORMAL);		// 生成
	cv::namedWindow("Depth", cv::WINDOW_GUI_NORMAL);
	cv::resizeWindow("RGB", 640, 480);			// サイズ設定
	cv::resizeWindow("Depth", 640, 480);
	cv::moveWindow("RGB", 100, 0);				// 配置
	cv::moveWindow("Depth", 800, 0);
		/* ウィンドウの準備は省略可能ですが，
			実行時の操作（再配置など）が面倒になるので，
			ソースに記述しておくべきでしょう．*/

	while (1) {
		if (cv::getWindowProperty("RGB", cv::WND_PROP_VISIBLE) < 1.0) break;
		if (cv::getWindowProperty("Depth", cv::WND_PROP_VISIBLE) < 1.0) break;

		// カラー画像の取得
//		cv::Mat color = freenect_sync_get_rgb_cv2(0);
		cv::Mat color = getColor();
		if (!color.data) fatal("キャプチャに失敗：カラーカメラ");

		// デプス画像の取得（カラー画像に位置合わせ版）
//		cv::Mat depth = freenect_sync_get_depth_cv2(0, FREENECT_DEPTH_REGISTERED);
		cv::Mat depth = getDepth();
		if (!depth.data) fatal("キャプチャに失敗：デプスカメラ");

		// カラー画像の表示
		cv::cvtColor(color, color, cv::COLOR_RGB2BGR);	// チャネル順序を変換
			// Kinect では RGB だが，OpenCV では BGR が基本なので．
		cv::imshow("RGB", color);

		// デプス画像の表示
		depth.convertTo(depth, -1, 32, 0);	// 5bit シフト（2^5 = 32 倍）
			/* 画素値16bitの内，深度値としては11bitしか使われない．
				そのまま表示しても暗すぎて，
				真っ黒な画像にしか見えないので輝度調整．*/
		cv::imshow("Depth", depth);

		// 操作
		int key = cv::waitKey(10);
		if (key == 'q') break;		// [Q]キーで終了へ
		if (key == '\e') break;		// [Esc]キーでも終了へ
	}
	// 終了
	freenect_sync_stop();		// キネクトを停止
		/* 省略可能だが，アプリ終了後にもデバイスは動作継続？
			測距用のレーザを出しっぱなしっぽい．節電．*/
	cv::destroyWindow("RGB");
	cv::destroyWindow("Depth");
	return (0);
}

