/*
Freenect x OpenCV のサンプルプログラム bgrm3.cc：顔検出による背景除去
コンパイル：$ c++ bgrm3.cc `pkgconf --cflags --libs libfreenect opencv` -lfreenect_sync -o bgrm3
実行：$ ./bgrm3 &
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

// 背景除去・前景抽出関数
// color, depth：入力画像
// fg：出力画像（前景，foreground）
// td：深度の閾値[mm]（前景／背景の境界深度，threshold depth）

// cv::Mat.at()版：お手軽
void removeBG(cv::Mat color, cv::Mat depth, cv::Mat &fg, ushort td)
{
	if (fg.data != color.data) fg = cv::Mat(color.size(), CV_8UC3);

	for (int y = 0; y < color.rows; y++)
	for (int x = 0; x < color.cols; x++) {
		ushort	d = depth.at<ushort>(y, x);
		if (d == 0) {		// 距離不明なら...
			fg.at<cv::Vec3b>(y, x) = cv::Vec3b(0x7F, 0x7F, 0x7F);	// 灰色に
		} else if (d > td) {	// 遠距離なら背景なので...
			fg.at<cv::Vec3b>(y, x) = cv::Vec3b(0x7F, 0x7F, 0x7F);	// 灰色に
//			fg.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);	// 黒に
		} else {		// 近距離なら前景なので
			fg.at<cv::Vec3b>(y, x) = color.at<cv::Vec3b>(y, x);
						// カラー画像の画素値に
		}
	}
}

// ポインタ版：（多分）高速
void removeBG2(cv::Mat color, cv::Mat depth, cv::Mat &fg, ushort td)
{
	if (fg.data != color.data) fg = cv::Mat(color.size(), CV_8UC3);

	cv::Vec3b	*f = (cv::Vec3b *)(fg.data);	// 出力カラー画素へのポインタ
	cv::Vec3b	*c = (cv::Vec3b *)(color.data);	// 入力カラー画素へのポインタ
	ushort		*d = (ushort *)(depth.data);	// 入力デプス画素へのポインタ

	cv::Vec3b	gray = cv::Vec3b(0x7F, 0x7F, 0x7F);	// 灰色
	cv::Vec3b	black = cv::Vec3b(0, 0, 0);		// 黒色

	for (int y = 0; y < color.rows; y++)
	for (int x = 0; x < color.cols; x++) {
		if (*d == 0) {		// 距離不明なら...
			*f = gray;
		} else if (*d > td) {	// 遠距離なら背景なので...
			*f = gray;		// 灰色に
//			*f = black;		// 黒に
		} else {		// 近距離なら前景なので
			*f = *c;		// カラー画像の画素値に
		}
		f++; c++; d++;	// 次の画素へ
	}
}

int main(int argc, char **argv)
{
	ushort	td = 2000;	// 深度の閾値[mm]（前景／背景の境界深度）
		// 決め打ちではなく，キーボード操作で変更できるようにしては？

	// ウィンドウの準備
	cv::namedWindow("RGB", cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("Depth", cv::WINDOW_GUI_NORMAL);
	cv::resizeWindow("RGB", 640, 480);
	cv::resizeWindow("Depth", 640, 480);
	cv::moveWindow("RGB", 100, 0);
	cv::moveWindow("Depth", 800, 0);

	cv::namedWindow("FG", cv::WINDOW_GUI_NORMAL);
	cv::resizeWindow("FG", 640, 480);
	cv::moveWindow("FG", 400, 300);

	// 物体検出の準備
	const char	*ccfile = "/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml";
		// カスケード分類器の定義ファイル
	cv::CascadeClassifier	cc;	// 分類器
	std::vector<cv::Rect>	objs;	// 検出物体（の四角形枠）の配列
	if (!cc.load(ccfile)) fatal("分類器の読み込み失敗");	// 分類器を読込・確認

	while (1) {
		if (cv::getWindowProperty("RGB", cv::WND_PROP_VISIBLE) < 1.0) break;
		if (cv::getWindowProperty("Depth", cv::WND_PROP_VISIBLE) < 1.0) break;
		if (cv::getWindowProperty("FG", cv::WND_PROP_VISIBLE) < 1.0) break;

		// カラー画像の取得
		cv::Mat color = getColor();
		if (!color.data) fatal("キャプチャに失敗：カラーカメラ");

		cv::cvtColor(color, color, cv::COLOR_RGB2BGR);	// チャネル順序を変換
				// fg への処理を省くため，先に変換しておく

		// デプス画像の取得（カラー画像に位置合わせ版）
		cv::Mat depth = getDepth();
		if (!depth.data) fatal("キャプチャに失敗：デプスカメラ");

		// 物体検出
		cc.detectMultiScale(color, objs);	// 分類器で物体を検出
		int	n = objs.size();		// 検出物体の個数

		// 閾値設定
		int	x0, y0;		// 中心座標
		if (n > 0) {	// 検出できた場合...顔の中心
			x0 = objs[0].x + objs[0].width/2;
			y0 = objs[0].y + objs[0].height/2;
		} else {	// できなかった場合...画像の中心
			x0 = color.cols/2;
			y0 = color.rows/2;
		}
		td = depth.at<ushort>(y0, x0) + (ushort)250;	// 中心デプス＋250mm を閾値に設定
			/* とりあえず，１人目の顔だけを利用してます．
				複数人に対応させるにはどうすれば？
					最も遠い人を基準にするとか？ */

		// 背景除去
		cv::Mat fg;
//		removeBG(color, depth, fg, td);
		removeBG2(color, depth, fg, td);

		// 顔検出の結果確認：背景除去の後じゃないと除去結果が汚れてしまいますよ
		for (int i = 0; i < n; i++) {
			cv::rectangle(color, objs[i], cv::Scalar(0, 0, 255), 3);
				// 検出物体の四角形枠（赤色，線幅３）を描画
				// （OpenCVのカラーは，RGBではなく，BGR形式）
		}

		// 表示
		cv::imshow("FG", fg);
		cv::imshow("RGB", color);
		depth.convertTo(depth, -1, 32, 0);	// 5bit シフト（2^5 = 32 倍）
		cv::imshow("Depth", depth);

		// 操作
		int	key = cv::waitKey(10);
		if (key == 'q') break;		// [Q]キーで終了へ
		if (key == '\e') break;		// [Esc]キーでも終了へ
	}
	// 終了
	freenect_sync_stop();		// キネクトを停止
		/* 省略可能だが，アプリ終了後にもデバイスは動作継続？
			測距用のレーザを出しっぱなしっぽい．節電．*/
	cv::destroyWindow("FG");
	cv::destroyWindow("RGB");
	cv::destroyWindow("Depth");
	return (0);
}

