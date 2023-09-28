/*
OpenCVのサンプルプログラム block.cc
	近傍処理（各画素の周辺ブロック毎の処理）の例：平滑化（ぼかし）フィルタ

コンパイル：$ c++ block.cc `pkg-config --cflags --libs opencv` -o block
実行：$ ./block [カメラ番号] &
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
	fprintf(stderr, "[ ] ブロックサイズリセット\n");
	fprintf(stderr, "[U] ブロックサイズアップ（ボケを強める）\n");
	fprintf(stderr, "[D] ブロックサイズダウン（ボケを弱める）\n");
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

/* 平滑化フィルタの例：画像src の各画素を中心とする
	辺長bsの正方形ブロック内の画素の平均値を画像dst に書込 */
// 実装例１：at()メソッド，低速...
void myblur1(const cv::Mat &src, cv::Mat &dst, int bs)
{
	if (dst.data != src.data) dst = cv::Mat(src.size(), CV_8UC3);
	// if pointernya menunjuk yang berbeda
    // mat::size return Size(cols, rows)
    // CV_8UC3 is an 8-bit unsigned matrix/image with 3 channels
    
	int	r = bs/2;	// ブロックの半径

	// 画素(x0, y0) に関するループ（y0行，x0列）
	for (int y0 = 0; y0 < src.rows; y0++)
	for (int x0 = 0; x0 < src.cols; x0++) {
    // mat.rows -> jumlah row
    // mat.cols > jumlah column

		// 画素(x0, y0) を中心とするブロック内画素(x, y) に関するループ
		int	x, y;
		int	n = 0;			// ブロック内の画素数のカウンタ
		cv::Vec3i	sum(0, 0, 0);	// ブロック内の合計のRGB値（int型 3ch）

		for (int dy = -r; dy <= r; dy++) {
			y = y0 + dy;
			if (y < 0) continue;		// ハミダシ禁止
			if (y > src.rows) break;
			// continue karena > dari itu masih mungkin 
			// break karena > tidak mungkin ada 
			
			for (int dx = -r; dx <= r; dx++) {
				x = x0 + dx;
				if (x < 0) continue;		// ハミダシ禁止
				if (x > src.cols) break;

				sum += src.at<cv::Vec3b>(y, x);	// ブロック内の合計
				n++;
			}
		}
		dst.at<cv::Vec3b>(y0, x0) = (cv::Vec3b)(sum/n);	// ブロック内の平均
	}
	/* なお，座標(x, y)（というか行番号y と列番号x）によって
		画素値にアクセスするメソッドat<cv::Vec3b>(y, x) は便利だが低速．
		画像やブロックのサイズが大きい場合，極端に遅くなる．
		実用上は，ポインタをうまく利用して実装し，高速化すべき． */
}

// 実装例２：高速版を定義せよ．
/*
void myblur2(const cv::Mat src, cv::Mat &dst, int bs)
{
	if (dst.data != src.data) dst = cv::Mat(src.size(), CV_8UC3);

	int	r = bs/2;

	cv::Vec3b	*p0 = (cv::Vec3b *)src.data;	// 入力画像のRGB値へのポインタ
	cv::Vec3b	*q0 = (cv::Vec3b *)dst.data;	// 出力画像のRGB値へのポインタ

	// 画素(x0, y0) に関するループ（y0行，x0列）
	for (int y0 = 0; y0 < src.rows; y0++) {
		for (int x0 = 0; x0 < src.cols; x0++) {
			...
			cv::Vec3b	*p = ...
			cv::Vec3b	*q = ...
			for (int dy = -r; dy <= r; dy++) {
				...
				for (int dx = -r; dx <= r; dx++) {
					...
				}
				p += ...
				q += ...
			}
			...
			*q = ...
			p0++;
			q0++
		}
		p0 += ...
		q0 += ...
	}
}
*/

// 上限値，下限値
#define	upLim(x, up)	((x > up) ? up : x)
#define	loLim(x, lo)	((x < lo) ? lo : x)

int main(int argc, char *argv[])
{
	const char	*window = "block operation";		// ウィンドウ名
	int	w = 640;	// 撮影画像サイズ
	int	h = 480;
	int	f = 0;		// 全画面フラグ
	int	bs = 1;		// ブロックサイズ
	int	dev = 0;	// カメラ番号（デバイスファイル/dev/videoN の数値N）
	if (argc > 1) dev = *argv[1] - '0';	// 複数カメラ切り替え

	// カメラの準備
	cv::VideoCapture	capture;	// キャプチャ元（RGBカメラ）
	if (!capture.open(dev)) fatal("カメラ起動に失敗");	// カメラを起動
	capture.set(cv::CAP_PROP_FRAME_WIDTH, w);	// 撮影画像サイズを設定
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, h);

	cv::namedWindow(window, cv::WINDOW_GUI_NORMAL);	// ウィンドウを生成
	cv::resizeWindow(window, w, h);			// ウィンドウサイズを変更

	while (1) {
		if (cv::getWindowProperty(window, cv::WND_PROP_VISIBLE) < 1.0) break;

		// 撮影
		cv::Mat	src;		// 入力画像
		if (!capture.read(src)) fatal("キャプチャに失敗");	// 撮影

		// 変換（平滑化）
		cv::Mat	dst;		// 出力画像
		// cv::blur(src, dst, cv::Size(bs, bs));	// 方法１：OpenCV版
		myblur1(src, dst, bs);			// 方法２：実装例１
//		myblur2(src, dst, bs);			// 方法３：実装例２

		// 表示
		cv::imshow(window, dst);

		// 操作
		int key = cv::waitKey(10);		// キー入力（待ち時間 10ms）
		if (key == 'q') break;		// [Q] で終了
		switch (key) {
		case 'h':			// [H]でヘルプ表示
			help();
			break;
		case 'f':			// [F]で全画面表示切替
			f = (f + 1)%2;
			cv::setWindowProperty(window, cv::WND_PROP_FULLSCREEN, f);
			break;
		case ' ': bs = 1; break;		// [ ]でブロックサイズリセット
		case 'u': 
            bs = upLim(bs + 2, 15); 
            break;	// [U]でブロックサイズアップ
		case 'd': bs = loLim(bs - 2, 1); break;		// [D]でブロックサイズダウン
		}
	}
	if (capture.isOpened()) capture.release();	// カメラを停止
	cv::destroyWindow(window);			// ウィンドウを破棄

	return (0);
}
/* 練習問題
(1) 変換について方法１〜２を実行し，どれも同じ結果となることを確認せよ．done
(2) 関数 cv::blur() について調査せよ．
(3) 実装例１の関数 myblur1() について，コードを調査・解読せよ． done
(4) 実装例２の関数 myblur2() を完成せよ．（上級者向け．）
*/

