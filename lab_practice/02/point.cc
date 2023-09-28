/*
OpenCVのサンプルプログラム point.cc：
	点処理（各画素毎の処理）の例（輝度調整）

コンパイル：$ c++ point.cc `pkg-config --cflags --libs opencv` -o point
実行：$ ./point [カメラ番号] &
*/
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

// using namespace cv;		// これで cv::〇〇 を単純に 〇〇 と書ける

// 強制終了関数
void fatal(const char *msg)
{
	perror(msg);		// エラーメッセージを表示
	exit(1);
}

// ヘルプ表示関数
void help() {
	fprintf(stderr, "========================\n");
	fprintf(stderr, "キー操作：\n");
	fprintf(stderr, "[H] ヘルプ表示\n");
	fprintf(stderr, "[ ] 輝度リセット\n");
	fprintf(stderr, "[U] 輝度アップ\n");
	fprintf(stderr, "[D] 輝度ダウン\n");
	fprintf(stderr, "[F] 全画面表示切替\n");
	fprintf(stderr, "[Q] 終了\n");
	fprintf(stderr, "========================\n");
}

/* 輝度調整関数の例：画像src の各画素の RGB値を s倍し画像dstに書込．
	要するに src.convertTo(dst, -1, s, 0.0) のクローン． */

/* 実装例１：二次元配列的なアクセス方式
	この方がコードは短いし，不連続データにも使えて万能ですが，
	実行速度は方法２より多少なりとも低下しますよ． */
void brighten2D(const cv::Mat &src, cv::Mat &dst, double s)
{
	/* 仮引数の & は参照渡し（メモリ領域共有）
		...この関数内での変数値の変化が呼出元にも反映される．*/

	if (dst.data != src.data) dst = cv::Mat(src.size(), CV_8UC3);	// dstを準備
		/* 入出力の画像データが異なる...つまり，まだ出力画像が作られていなければ...
			入力画像と同サイズの画像の容器を作成．
			入出力が同じなら作成不要．
			これで，brighten1(src, src, ...) としても使える． */
		/* CV_8UC3 は画像の形式マクロのひとつであり，
			8bit の uchar型が 3ch分で１画素としている．
			つまりカラー画像（BGR形式）を指定． */

	// 画素(x, y) に関するループ（y行，x列）
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++) {
		// rowsは行数（縦方向のサイズ），colsは列数（横方向のサイズ）

		dst.at<cv::Vec3b>(y, x) = src.at<cv::Vec3b>(y, x) * s;	// 各画素値を s倍
			/* Matは実のところ画像ではなく行列なので，画素の座標指定は，
				at<...>(x, y) ではなく，at<...>(y, x) であることに注意． */
			/* 画素値の型 cv::Vec3b は，要素数３で各要素が 1byte のベクトル型．
				要するに配列 uchar ○[3] を一括したもの．
				OpenCV のカラーは，RGBではなく，BGR形式なので，
				B値は (*c)[0]，G値は (*c)[1]，R値は (*c)[2] ですよ． */
	}
	// Vec3b型で 3ch一括ではなく，uchar型の配列で 1chずつ処理しても良い．
	// 形式マクロ CV_8UC3 等とデータ型 Vec3b 等との使い分けにも注意が必要．
}

// 実装例２：一次元配列的なアクセス方式
void brighten1D(const cv::Mat &src, cv::Mat &dst, double s)
{
	if (dst.data != src.data) dst = cv::Mat(src.size(), CV_8UC3);	// dstを準備

	cv::Vec3b	*p = (cv::Vec3b *)src.data;	// 先頭画素のRGB値へのポインタ
	cv::Vec3b	*q = (cv::Vec3b *)dst.data;

	int	n = src.rows * src.cols;	// 画素数
	for (int i = 0; i < n; i++) {		// 全要素について反復
		*q = (cv::Vec3b)((cv::Vec3d)(*p) * s);	// RGB値をs倍
			// cv::Vec3d は，要素数3 で各要素double型のベクトル型
		p++;		// 次の画素へ
		q++;
	}
	/* 画像データは，人間の脳内では二次元配列なので
		二重ループを使いたくなりますが...，
		コンピュータの内部的には一次元配列なので一重ループでOK．
		この方が高速に実行される．
		ただし，データ全体がメモリ内で連続している場合を仮定してますよ．
		ROI（部分画像）とかでは不連続なので，この技は使えません．*/
}
/* 効率性・万能性の両立には，二次元と一次元の合わせ技が良さげ．
	外側ループでは p = src.at(y, 0) 等で各行の先頭アドレスを計算しておき，
	内側ループでは p++ 等で連続する画素値にアクセスする，みたいな． */

// Backup
void grayScale(const cv::Mat &src, cv::Mat &dst) {
	// if(dst.data != src.data) 
	dst = cv::Mat(src.size(), CV_8UC1);

	cv::Vec3b *p = (cv::Vec3b *)(src.data);
	uchar *q = (uchar *)(dst.data); // salahnya adalah outputnya jadikan 1 channel dong 
	// berarti ini gak jadi vector lagi

	double s[3] = {0.1, 0.6, 0.3};
	int n = src.rows * src.cols;

	for (int i = 0; i < n; i ++) {
		*q = (uchar)((*p)[0] * s[0]);
		*q += (uchar)((*p)[1] * s[1]); 
		*q += (uchar)((*p)[2] * s[2]);
		// https://stackoverflow.com/questions/26480548/opencv-calculating-new-red-pixel-value <- refer
		// std::cout << *p << std::endl;
		p++;
		q++;
	}
}

// void grayScale(const cv::Mat &src, cv::Mat &dst) {
// 	if (dst.data != src.data) dst = cv::Mat(src.size(), CV_8UC1);	
// 	for (int y = 0; y < src.rows; y++){
// 		unsigned char tmp = 0;
// 		for (int x = 0; x < src.cols; x++) {
// 			tmp += src.at<cv::Vec3b>(y, x)[0] * 0.0;	
// 			tmp += src.at<cv::Vec3b>(y, x)[1] * 1.0;	
// 			tmp += src.at<cv::Vec3b>(y, x)[2] * 1.0;	
// 			dst.at<cv::Vec<uchar, 1>(y, x) = tmp;
// 		}
// 	}
// }

// 上限値，下限値
#define	upLim(x, up)	((x > up) ? up : x)
#define	loLim(x, lo)	((x < lo) ? lo : x)

int main(int argc, char *argv[])
{
	const char	*window = "point operation";		// ウィンドウ名
	int	w = 640;	// 撮影画像サイズ
	int	h = 480;
	int	f = 0;		// 全画面フラグ
	double	s = 2.5;	// 輝度の倍率
	int	dev = 0;	// カメラ番号（デバイスファイル/dev/videoN の数値N）
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

		// 撮影
		cv::Mat	src;		// 入力画像
		if (!capture.read(src)) fatal("キャプチャに失敗");

		// 変換（輝度調整）
		cv::Mat	dst;		// 出力画像
		// src.convertTo(dst, -1, s, 0.0);	// 方法１：OpenCV版
		//			output, rtype, alpha, beta
		// rtype -1 means the output array has sam bit depth as the source
		// beta -20 decrease the brightness by 20 for each pixel (not percentage)
		// convertTo
		// m(x,y) = saturate_cast<rType>(α(∗this)(x,y)+β)

		// brighten2D(src, dst, s);	// 方法２：実装例１（二次元アクセス方式）

		// brighten1D(src, dst, s);	// 方法３：実装例２（一次元アクセス方式）
			// dst なしで，入出力とも src にしても OK

		// cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);

		grayScale(src, dst);


		// 表示
		cv::imshow(window, dst);

		// 操作
		int key = cv::waitKey(10);	// キー入力（待ち時間 10ms）
		if (key == 'q') break;		// [Q] で終了
		switch (key) {
		case 'h':			// [H]でヘルプ表示
			help();
			break;
		case 'f':			// [F]で全画面表示切替
			f = (f + 1)%2;
			cv::setWindowProperty(window, cv::WND_PROP_FULLSCREEN, f);
			break;

		case ' ': s = 1.0; break;	// [ ]で輝度リセット
		case 'u': s = upLim(s + 0.2, 4.0); break;	// [U]で輝度アップ
		case 'd': s = loLim(s - 0.2, 0.0); break;	// [D]で輝度ダウン
			/* cv::waitKey() は文字入力専用．
				特殊キー（カーソル等）入力には cv::waitKeyEx() が必要． */
		}
	}
	if (capture.isOpened()) capture.release();	// カメラを停止
	cv::destroyWindow(window);			// ウィンドウを破棄

	return (0);
}

/* 練習問題
(1) 変換について方法１〜３を実行し，どれも同じ結果となることを確認せよ．same 
(2) オブジェクトMat のメソッドconvertTo() および at() について調査せよ．(at gk nemu)
(3) 輝度調整関数の実装例１および２について，コードを調査・解読せよ． done
(4) cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY) のクローン bgr2gray(src, dst) を実装せよ．
	なお，dstについてはモノクロなので，画素値は uchar（＝unsigned char）型，
	形式マクロは CV_8C1 などとしよう．（ベクトルではない．）
	RGB からグレー輝度値 I への変換式は I = R*0.3 + G*0.6 + B*0.1 とかでOK．
*/