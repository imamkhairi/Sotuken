/*
OpenCVのサンプルプログラム detect.cc：画像変換フィルタ
	（convert.cc を元に10行程度だけ追加・変更．）

コンパイル：$ c++ `pkg-config --cflags --libs opencv` detect.cc -o detect
実行：$ ./detect [カメラ番号] &
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
//	fprintf(stderr, "[Space] 変換機能切替\n");
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
	const char	*window = "detect";	// ウィンドウ名
	int	w = 640;	// 撮影画像サイズ
	int	h = 480;
	int	f = 0;		// 全画面フラグ
	int	dev = 0;	// カメラ番号（デバイスファイル/dev/videoN の数値N）
	if (argc > 1) dev = *argv[1] - '0';	// 複数カメラ切り替え

	// 物体検出の準備
	const char	*ccfile = "/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml";
		// カスケード分類器の定義ファイル
		// frontalface は正面顔検出用，このディレクトリ内に他にもある
		// 残念ながら，マスク着用状態では認識しづらい...

	cv::CascadeClassifier	cc;	// 分類器
	std::vector<cv::Rect>	objs;	// 検出物体（の四角形枠）の配列
		// std は C++ の標準ライブラリの名前空間
		// cv::Rect は四角形オブジェクト
		// cv::vector is sequence container that resize it self automatically
		// <> determine what kind of data will be stored
		// cv::Rect (x, y, width, height)
	
	if (!cc.load(ccfile)) fatal("分類器の読み込み失敗");	// 分類器を読込・確認

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
		cv::Mat	src;
		if (!capture.read(src)) fatal("キャプチャに失敗");

		cv::Mat roi_src;
		cv::Mat roi_clone;


		// 物体検出
		cc.detectMultiScale(src, objs);	// 分類器で物体を検出
		//				input image, rect (pake vektor karena bisa lebih 1 yang di deteksi)

		int	n = objs.size();		// 検出物体の個数
		for (int i = 0; i < n; i++) {
			// cv::rectangle(src, objs[i], cv::Scalar(0, 0, 255), 2);
					//    img, pt..(dalam bentuk vector), color, thickness
				// cv.rectangle(	img, pt1, pt2, color[, thickness[, lineType[, shift]]]	)
				// 検出物体の四角形枠（赤色，線幅３）を描画
				// （OpenCVのカラーは，RGBではなく，BGR形式）
				// Scalar is Template class for a 4-element vector derived from Vec. 
				// Vec is template class short numerical vectors (imagine linear algebra)
			
			roi_src = cv::Mat(src, objs[i]);
			// roi_clone = cv::Mat(src, objs[i]).clone();
						//    source, rect
			cv::GaussianBlur(roi_src, roi_src, cv::Size(), 10);
			roi_src.copyTo(src(objs[i]));

			// cv::Mat .copyTo(cv::Mat) -> copy mentah2 artinya bener2 diganti semua
			// cv::Mat .copyTo(cv::Mat(cv::Rect(a,b,c,d))) -> copy ke bagian rect nya aja

		}

		// 表示
		cv::imshow(window, src);

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
		}
	}
	if (capture.isOpened()) capture.release();	// カメラを停止
	cv::destroyWindow(window);			// ウィンドウを破棄

	return (0);
}
/* 練習問題
(1) 画像の検出部分にだけ画像処理フィルタ（ぼかし cv::blur(...) 等）を適用せよ．
	（OpenCV には画像の一部だけを処理対象とする機能 done
	「ROI: region of interest」が用意されている．）

(2) 顔チェンジャ（複数人の顔領域を互いに交換・表示する）機能を追加せよ．
	（上級者向け．）INI BELUM
*/

