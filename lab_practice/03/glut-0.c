/*
GLUT の基本コード glut-0.c
	ウィンドウだけの表示... コールバックについて理解しよう
コンパイル：$ cc glut-0.c -lGL -lglut -o glut-0
実行：$ ./glut-0
操作：[Q] で終了，[-] で背景を暗く，[+] で背景を明るく
*/
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>

int window_w = 640;			// ウィンドウの幅
int window_h = 480;			// ウィンドウの高さ
double bright = 1.0;			// 背景の明るさ

// シーン描画のコールバック関数
// （描画要求があった時，glutMainLoop() から自動的に呼び出されるように，
// main() 内の glutDisplayFunc() で指定した．）
void scene()
{
	static double	r = 0.5;		// 背景色のR成分
	static double	g = 0.4;		// 背景色のG成分
	static double	b = 1.0;		// 背景色のB成分
	static double	a = 1.0;		// 背景色のA成分

	glClearColor(r*bright, g*bright, b*bright, a);	// 背景色を設定
		// 				r,　g,　b,　a
		// specifies clear values for the color buffers

	glClear(GL_COLOR_BUFFER_BIT);		// 画面を消去
		// clear buffers to preset values ?

	// ... 後でこの辺りにモデルを追加 ...

	glFlush();				// 画面を更新
		// clear the buffer so that new conditioin can be displayed in the screen?
}

// キー入力のコールバック関数
// （キー入力があった時，glutMainLoop() から自動的に呼び出されるように，
// main() 内の glutKeyboardFunc() で指定した．）
// key：キー文字
// x, y：マウスポインタの座標
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': exit(0); break;		// 終了
	case '-': bright -= 0.2; break;		// 背景色を暗く
	case '+': bright += 0.2; break;		// 背景色を明るく
	}
	glutPostRedisplay();			// 再描画を要求
		// marks the current window as needing to be redisplayed 
}

int main(int argc, char **argv)
{
	// ウィンドウの設定
	glutInit(&argc, argv);			// GLUT を初期化
		// initiate glut
		// glut -> initialize glut library
		// window definition, window control, and monitoring of keyboard and mouse input.

	glutInitDisplayMode(GLUT_RGBA);		// 画面を設定（カラー）
		// 				mode(refer documentation)
		// set initial display mode

	glutCreateWindow("GLUT: window");	// ウィンドウを生成
		// 				name 
		// create window

	glutReshapeWindow(window_w, window_h);	// ウィンドウサイズを設定
		// reshape,   	

	// コールバックの設定
		// A callback function is a function which the library (GLUT) calls when it needs to know how to proccess something.
		// e.g. when glut gets a key down event it uses the glutKeybourdFunc callback routine to find out what to do with a key press.
	
		// function that used here doesn't need to passed any parameter even bracket
	glutDisplayFunc(scene);		// シーン描画関数の登録
		// sets the display callback for the current window
		// which means set how the window should displayed
	glutKeyboardFunc(keybd);	// キー入力関数の登録
		// so that the library know how to process when get keyboard input

		// refer to documentation for more callback function



	// メインループ
	glutMainLoop();			// 描画・入力の開始
		/* glutMainLoop() の内部から登録したコールバック関数
			scene() や keybd() が自動的に呼び出されますよ．

			// メインループ内の処理の概要
			void glutMainLoop()
			{
				...
				while (1) {
					...
					if (描画要求あり) scene();
					マウス座標(&x, &y);
					key = 入力キー();
					if (キー入力あり) keybd(key, x, y);
					...
				}
			}
		*/
	return (0);
}

