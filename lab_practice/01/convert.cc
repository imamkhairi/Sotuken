/*
OpenCVのサンプルプログラム convert.cc：画像変換フィルタ
	（camera.cc を元に数行だけ追加・変更．）

コンパイル：$ c++ `pkg-config --cflags --libs opencv` convert.cc -o convert
実行：$ ./convert [カメラ番号] &
注意：終了は[Q]キー．
	ウィンドウクローズボタンでは完全には終了しない．
*/
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

// using namespace cv;		// これで cv::〇〇 を単純に 〇〇 と書ける

// ヘルプ表示関数
void help() {
	fprintf(stderr, "========================\n");
	fprintf(stderr, "キー操作：\n");
	fprintf(stderr, "[H] ヘルプ表示\n");
	fprintf(stderr, "[F] 全画面表示切替\n");
	fprintf(stderr, "[Space] 変換機能切替\n");
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
	const char	*window = "convert";	// ウィンドウ名
	int	w = 640;	// 撮影画像サイズ
	int	h = 480;
	int	f = 0;		// 全画面フラグ
	int	dev = 0;	// カメラ番号（デバイスファイル/dev/videoN の数値N）
	bool effect = false;
    if (argc > 1) dev = *argv[1] - '0';	// 複数カメラ切り替え

	// カメラの準備
	cv::VideoCapture	capture;
	if (!capture.open(dev)) fatal("カメラ起動に失敗");
	capture.set(cv::CAP_PROP_FRAME_WIDTH, w);
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, h);

	// ウィンドウの準備
	cv::namedWindow(window, cv::WINDOW_GUI_NORMAL);
	cv::resizeWindow(window, w, h);

	while (1) {
		if (cv::getWindowProperty(window, cv::WND_PROP_VISIBLE) < 1.0) break;
							// 窓が閉じたら終了
		// 撮影
		cv::Mat	src;			// 画像データ（変換前）
		if (!capture.read(src)) fatal("キャプチャに失敗");

		// 変換
		cv::Mat	dst;			// 画像データ（変換後）
		//cv::flip(src, dst, 1);	// 鏡像変換（1: 左右方向）
			// src の内容を変換して dst に格納
		cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
			// モノクロ化：カラー画像 → グレースケール画像

		// 表示
        if(effect == true) {
            cv::imshow(window, dst);	// 変換後の画像
        } else {
            cv::imshow(window, src);	// 変換前の画像
        }

		// 操作
		int key = cv::waitKey(10);	// キー入力（待ち時間 10ms）
		if (key == 'q') break;		// [Q]で終了
		switch (key) {
		case 'h':		// [H]でヘルプ表示
			help();
			break;
		case 'f':		// [F]で全画面表示切替
			f = (f + 1)%2;		// f = 0 → 1 → 0 → 1 → ...
			cv::setWindowProperty(window, cv::WND_PROP_FULLSCREEN, f);
			break;
        case ' ': 
            effect = (effect == true) ? false : true;
        }
	}

	if (capture.isOpened()) capture.release();	// カメラを停止
	cv::destroyWindow(window);			// ウィンドウを破棄

	return (0);
}
/* 練習問題
(1) cv::flip(...) および cv::imshow(...) の dst を src に変えるとどうなる？
	（要するに src 自身へ変換・コピーできるか？変数を節約できるか？）done

(2) cv::flip(...) と交換（あるいは追加）して，
	cv::cvtColor(...) を試用せよ．done

(3) 画像変換機能のon/off等について
	キーボード操作による切り替えを可能にせよ． done

(4) OpenCV には他にどのような画像変換関数があるか？調査・試用せよ． done
*/ 

