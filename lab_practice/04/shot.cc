/* OpenCV × Zenity で写真撮影アプリ shot.c
コンパイル：$ c++ shot.cc `pkgconf --cflags --libs opencv` -o shot
実行：$ ./shot &
操作：[H] ヘルプ，[Space] 撮影，[Q] 終了
*/
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
	/* 「cv::」を省略するけど...
		OpenCVの関数と他の関数との見分けに注意．*/

// 強制終了関数
void fatal(const char *msg)
{
	perror(msg);
	exit(1);
}

#define QUIT_GUI	"zenity --question --text='本当に止めんの？'"
#define HELP_GUI	"zenity --text-info --filename=shot.txt --title=Help"
#define FILE_GUI	"zenity --file-selection --save --confirm-overwrite --filename=shot.png --file-filter='*.png *.jpg'"
#define SOUND_CMD	"play /usr/share/sounds/freedesktop/stereo/camera-shutter.oga &"
#define	BUF_LEN	256

// ヘルプ表示GUI
void help(void)
{
	system(HELP_GUI);	// シャッター音再生
}

// ファイル選択GUI
int select(char *file, int n)
{
	FILE	*pp = popen(FILE_GUI, "r");	// ファイル選択ダイアログを開く
	if (!pp) return (1);	// 起動失敗

	if (fscanf(pp, "%s", file) != 1) return (1);
				// ダイアログからファイル名を入力
	pclose(pp);		// ダイアログを閉じる
	return (0);
}

// ファイル保存GUI
void save(Mat &img)
{
	system(SOUND_CMD);		// シャッター音再生

	char	file[BUF_LEN];		// ファイル名のバッファ
	if (select(file, BUF_LEN) != 0) return;	// ファイル選択
		// based on declaration above 0 means successful 

	imwrite(file, img);		// 画像ファイル保存
}

// 終了確認GUI
int confirm(void)
{
	return (system(QUIT_GUI));	// 終了確認ダイアログ
				// yes で 0，no で 1 を返す
}
 
int main(int argc, char *argv[])
{
	const char	*win = "shot";
	int	w = 640;
	int	h = 480;
	int	dev = 0;
	if (argc > 1) dev = *argv[1] - '0';

	VideoCapture	cap;
	if (!cap.open(0)) fatal("カメラ起動に失敗");
	cap.set(CAP_PROP_FRAME_WIDTH, w);
			// set width of the frames in the video stream
	cap.set(CAP_PROP_FRAME_HEIGHT, h);
			// set height of the frames in the video stream

	namedWindow(win, WINDOW_GUI_NORMAL);
	resizeWindow(win, w, h);

//	while (getWindowProperty(win, WND_PROP_VISIBLE) > 0.0) {
	while (1) {
		Mat	src;
		if (!cap.read(src)) fatal("キャプチャに失敗");	// 撮影

		imshow(win, src);		// 表示

		int	key = waitKey(10);	// キー入力（待ち時間 10ms）
		if ((getWindowProperty(win, WND_PROP_VISIBLE) < 1.0) || (key == 'q')) {	// 窓閉ボタン or [q]キーで...
			// 				current window, prop id
			// check whether the window exists and is visible
			if (confirm() == 0) break;	// 終了確認 
				// yes = 0
				// no = 1
		}
		switch (key) {
		case ' ': save(src); break;	// 画像ファイル保存
		case 'h': help(); break;	// ヘルプ表示
		}
	}
	if (cap.isOpened()) cap.release();	// カメラを停止
	destroyWindow(win);			// ウィンドウを破棄

	return (0);
}

