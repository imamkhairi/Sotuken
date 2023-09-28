/*
OpenCVのサンプルプログラム camera.cc：ビデオカメラモニタ

コンパイル：$ c++ `pkg-config --cflags --libs opencv` camera.cc -o camera
実行：$ ./camera [カメラ番号] &
注意：終了は[Q]キー．
	ウィンドウクローズボタンでは完全には終了しない．
*/
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;		// これで cv::〇〇 を単純に 〇〇 と書ける

// ヘルプ表示関数
void help() {
	fprintf(stderr, "========================\n");
	fprintf(stderr, "キー操作：\n");
	fprintf(stderr, "[H] ヘルプ表示\n");
	fprintf(stderr, "[F] 全画面表示切替\n");
	fprintf(stderr, "[Q] 終了\n");
	fprintf(stderr, "========================\n");
}

// 強制終了関数
void fatal(const char *msg)
{
	perror(msg);		// エラーメッセージを表示
	exit(1);
}

int main(int argc, char *argv[])
{
	const char	*window = "camera";	// ウィンドウ名
	int	w = 640;	// 撮影画像サイズ
	int	h = 480;
	int	f = 0;		// 全画面フラグ
	int	dev = 0;	// カメラ番号（デバイスファイル/dev/videoN の数値N）
	if (argc > 1) dev = *argv[1] - '0';	// 複数カメラ切り替え

	// カメラの準備（方法１）
//	cv::VideoCapture	capture;	// キャプチャを生成
//	if (!capture.open(dev)) fatal("カメラ起動に失敗");	// カメラを起動・確認

	// カメラの準備（方法２）
	VideoCapture	capture(dev);	// キャプチャを生成，カメラを起動
	if (!capture.isOpened()) fatal("カメラ起動に失敗");	// カメラを確認

	capture.set(cv::CAP_PROP_FRAME_WIDTH, w);	// 撮影画像サイズを設定
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, h);
		/* サイズ未指定のデフォルトでは，おそらく，カメラ自身の最大解像度となるので，
			画像処理の負担が重くなりすぎてしまうでしょう... */

	// ウィンドウの準備
//	cv::namedWindow(window, cv::WINDOW_GUI_NORMAL);	// 生成方法１：GUI 無
	namedWindow(window, WINDOW_NORMAL);	// 生成方法２：サイズ可変
//	cv::namedWindow(window, cv::WINDOW_AUTOSIZE);	// 生成方法３：サイズ固定

	resizeWindow(window, w, h);	// ウィンドウサイズを変更

	while (1) {			// メインループ
		if (getWindowProperty(window, WND_PROP_VISIBLE) < 1.0) break;
							// 窓が閉じたら終了

		Mat	src;			// 画像データ

		// 撮影・確認（方法１）
//		capture.read(src);				// 撮影
//		if (src.empty()) fatal("キャプチャに失敗");	// 確認

		// 撮影・確認（方法２）
		if (!capture.read(src)) fatal("キャプチャに失敗");	// 撮影・確認

		// 表示
		imshow(window, src);

		// 操作
		int key = waitKey(10);	// キー入力（待ち時間 10ms）
		if (key == 'q') break;		// [Q]で終了
		
		switch (key) {
		case 'h':		// [H]でヘルプ表示
			help();
			break;
		case 'f':		// [F]で全画面表示切替
			f = (f + 1)%2;		// f = 0 → 1 → 0 → 1 → ...
			setWindowProperty(window, WND_PROP_FULLSCREEN, f);
			break;
		}
	}
	if (capture.isOpened()) capture.release();	// カメラを停止
	destroyWindow(window);			// ウィンドウを破棄

	return (0);
}
/* 練習問題
(1) カメラの準備方法２通りについて試行せよ．（記述は異なるが動作は同じ．）done
(2) ウィンドウの生成方法３通りについて試行せよ．（動作も異なる．）done 
(3) 撮影・確認の方法２通りについて試行せよ．（記述は異なるが動作は同じ．）done 
(4) namespace を利用して，ソースをコンパクトに書き換えよ．done
*/

