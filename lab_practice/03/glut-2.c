/*
GLUT の基本コード glut-2.c
	アニメーション...
		動作速度を一定化しよう
		ウィンドウサイズ変更に対応しよう
コンパイル：$ cc glut-2.c -lGL -lGLU -lglut -o glut-2
実行：$ ./glut-2
操作：[Q] で終了
*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

int window_w = 640;		// ウィンドウの幅
int window_h = 480;		// ウィンドウの高さ
// int interval = 100;		// 画面更新の時間間隔（msec）
int interval = 33;		// 画面更新の時間間隔（msec）
double angle = 0.0;		// 急須の回転角度

// オブジェクトの設定（glut-1.c と同）
void object()
{
	static GLfloat diff[] = {0.8, 0.4, 0.4, 1.0};	// 拡散反射係数 RGBA
	static GLfloat spec[] = {0.4, 0.4, 0.4, 1.0};	// 鏡面反射係数 RGBA
	static GLint hl = 10;			// ハイライトパラメータ

	// 物体色（反射係数等）の設定
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, hl);	

	// 物体の配置（合成変換を逆順に指定すること!!）
	// 回転する急須
	glPushMatrix();			// グループ化の開始
		glRotated(angle, 0.0, 1.0, 0.0);	// 物体を回転
		glTranslated(0.0, 0.0, 30.0);		// 物体を平行移動
		glutSolidTeapot(10.0);			// 物体を原点に配置
	glPopMatrix();			// グループ化の終了

	// 四角柱
	glPushMatrix();
		glScaled(10.0, 30.0, 10.0);		// 物体を拡大
		glutSolidCube(1.0);			// 物体を原点に配置
	glPopMatrix();
}

// 照明の設定（glut-1.c と同）
void light()
{
	static GLfloat col[] = {1.0, 1.0, 1.0, 1.0};	// 照明光の色 RGBA
	static GLfloat pos[] = {0.0, 1.0, 0.5, 0.0};	// 照明光線の方向

	glEnable(GL_LIGHTING);			// 陰影付け開始
	glEnable(GL_LIGHT0);			// 光源を有効化
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);	// 直接光の色を設定
	glLightfv(GL_LIGHT0, GL_AMBIENT, col);	// 間接光の色を設定
	glLightfv(GL_LIGHT0, GL_POSITION, pos);	// 光源を配置
}

// カメラの設定（glut-1.c と同）
void camera()
{
	double	fovy = 30.0;		// y軸方向の視野角
	double	near = 1.0;		// 視界の最近端までの距離
	double	far = 10000.0;		// 視界の最遠端までの距離

	// カメラの視野の設定
	glMatrixMode(GL_PROJECTION);	// 透視投影を開始
	glLoadIdentity();		// 変換行列をリセット
	gluPerspective(fovy, (double)window_w/(double)window_h, near, far);
					// 投影パラメータを設定
	// カメラの位置の設定
	glMatrixMode(GL_MODELVIEW);	// モデリングを開始
	glLoadIdentity();		// 変換行列をリセット
	gluLookAt(0.0, 30.0, 100.0,	// カメラの位置
			  0.0, 0.0, 0.0,		// 注視点の位置
			  0.0, 1.0, 0.0);		// カメラの上方向
}

// 描画要求 glPostRedisplay() 等によるコールバック関数
void scene()
{
	glClearColor(0.0, 1.0, 1.0, 1.0);	// 背景色設定
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				// カラーバッファとデプスバッファを消去

	glViewport(0, 0, window_w, window_h);
				// 表示領域を設定（ウィンドウ全体）

	camera();				// カメラを設定
	light();				// 照明を設定
	object();				// 物体を設定

	glutSwapBuffers();			// 画面を更新
}

// キー入力のコールバック関数
// key：キー文字
// x, y：マウスポインタの座標
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': exit(0); break;
	}
}

// ウィンドウサイズ変更によるコールバック関数
// w, h：ウィンドウサイズ
void window(int w, int h)
{
	window_w = w;			// サイズを記憶
	window_h = h;
}

// アニメーションのコールバック関数
void anim(int data)
{
	angle += 2.0;				// 回転角度を変更
	if (angle > 360.0) angle -= 360.0;

	glutPostRedisplay();			// 再描画を要求
	glutTimerFunc(interval, anim, 0);	// コールバックを再登録
}

int main(int argc, char **argv)
{
	// ウィンドウの設定
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
				// カラー，隠面処理，裏画面を使う
	glutCreateWindow("GLUT: animation");
	glutReshapeWindow(window_w, window_h);

	// コールバックの設定
	glutDisplayFunc(scene);		// シーン描画関数の登録
	glutKeyboardFunc(keybd);	// キー入力関数の登録
	glutReshapeFunc(window);		// サイズ変更関数の登録
		// if enabled size of object that contained in the window will change relative to the window
	glutTimerFunc(interval, anim, 0);	// アニメーション関数の登録
		// 		 interval(ms), callback function for animation, value that will be passed to the func
		// 時間 interval 待機後に関数 anim(0) を呼出
		//  registers a timer callback to be triggered in a specified number of milliseconds.

	// レンダリングの設定
	glEnable(GL_DEPTH_TEST);	// 隠面処理
	glEnable(GL_NORMALIZE);		// 法線正規化

	// メインループ
	glutMainLoop();		// アニメーション開始
	return (0);
}

