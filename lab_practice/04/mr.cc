// OpenGL × OpenCV で複合現実 mr.c
// 仮想世界と現実世界の融合
// コンパイル：$ c++ mr.cc `pkgconf --cflags --libs opencv glut glu gl` -o mr
// 実行：$ ./mr
// 操作：[Q] で終了，[↑/↓/←/→] でモニタを回転
// 	[F] で全画面化，[W] でウィンドウ化，

#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int	window;			// ウィンドウID
int	window_w = 640;		// ウィンドウの幅
int	window_h = 480;		// ウィンドウの高さ

using namespace	cv;		// 「cv::」を省略しますよ．
VideoCapture	cap;		// 現実カメラ
int		cam = 0;	// 現実カメラ番号

double	angle = 0.0;	// 急須の回転角度
double	tilt = 0.0;	// モニタの回転角度
double	rot = 0.0;
int	timer = 10;	// 映像更新時間(msec)
GLuint	tex[10];	// テクスチャID とりあえず10個分

GLfloat red[] = {0.8, 0.4, 0.4, 1.0};
GLfloat green[] = {0.4, 0.8, 0.4, 1.0};
GLfloat blue[] = {0.4, 0.4, 0.8, 1.0};

GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
GLfloat black[] = {0.0, 0.0, 0.0, 1.0};

// 異常終了
void fatal(const char *msg)
{
	perror(msg);
	exit(1);
}

// 正常終了
void fin()
{
	if (cap.isOpened()) cap.release();	// カメラを停止
	glutDestroyWindow(window);		// ウィンドウを破棄
		// このアプリのウィンドウは OpenCV ではなく GLUT で作ってました．
		// 破棄にも GLUT の関数を使います．cv::destroyWindow() ではなく．
	exit(0);
}

// モニタのスクリーン部分の設定
void monitorScreen(double x, double y)
{
	static Mat	frame;		// 現実映像

	if (!cap.read(frame)) fatal("キャプチャ失敗");	// 現実映像取得
	cvtColor(frame, frame, COLOR_BGR2RGB);
		// 色成分の順序変換：BGR（OpenCVの標準）→ RGB（OpenGLの標準）

	glBindTexture(GL_TEXTURE_2D, tex[0]);
		// 		GLenum target, GLuint texture
		// texture -> Must have the value GL_TEXTURE_1D or GL_TEXTURE_2D.
		// function enables the creation of a named texture(tex[0]) that is bound to a texture target. 
		// bind a named texture to a texturing target

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, frame.cols, frame.rows, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	// GLenum target, GLint  components, GLint  width, GLInt  height, GLenum format, GLenum type, const void   *data
			// 現実映像をテクスチャ画像として登録
			// creates 2d mipmaps
			// Mipmaps are smaller, pre-filtered versions of a texture image, representing different levels of detail (LOD) of the texture.
			// builds a series of prefiltered two-dimensional texture maps of decreasing resolutions called a mipmap. 
			// This is used for the antialiasing of texture-mapped primitives. 

	// 現実映像をテクスチャマッピングにより表示
	glDisable(GL_LIGHTING);		// 陰影付け停止
	glEnable(GL_TEXTURE_2D);	// マッピング開始
	glBegin(GL_QUADS);	// 四角形（スクリーン）描画開始
		glTexCoord2d(0.0, 0.0); glVertex3d(-x,  y, 0.0);	// 左上
		glTexCoord2d(0.0, 1.0); glVertex3d(-x, -y, 0.0);	// 左下
		glTexCoord2d(1.0, 1.0); glVertex3d( x, -y, 0.0);	// 右下
		glTexCoord2d(1.0, 0.0); glVertex3d( x,  y, 0.0);	// 右上
		// TexCoord で画像の座標と Vertex で図形の座標とを対応付けている
	glEnd();
	glDisable(GL_TEXTURE_2D);	// マッピング終了
}

// モニタの本体部分の設定
void monitorBody(double w, double h, double d)
{
	// 物体色（反射係数等）の設定
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);	// 拡散反射係数
	glMaterialfv(GL_FRONT, GL_SPECULAR, gray);		// 鏡面反射係数
	glMateriali(GL_FRONT, GL_SHININESS, 1);			// ハイライトパラメータ

	// 描画
	glEnable(GL_LIGHTING);		// 陰影付け開始
	glPushMatrix();
		glScaled(w, h, d);
		glTranslated(0.0, 0.0, -0.51);
		glutSolidCube(1.0);
	glPopMatrix();
}

// 仮想TVモニタの設定
void monitor()
{
	static int	cw = cap.get(CAP_PROP_FRAME_WIDTH);	// 画像のサイズ
	static int	ch = cap.get(CAP_PROP_FRAME_HEIGHT);
	static double	sw = 20.0*cw/ch;	// スクリーンのサイズ
	static double	sh = 20.0;
	static double	sx = sw/2.0;		// スクリーンの頂点座標
	static double	sy = sh/2.0;
	static double	w = sw + 2.0;		// 本体のサイズ
	static double	h = sh + 2.0;
	static double	d = 5.0;
		// static 変数は，この関数内でデータが維持される．
		// 次回の呼出時にも変数を再生成・再設定せずに済むので効率的．

	glPushMatrix();
		glRotated(rot, 0.0, 1.0, 0.0);	// 回転
		glRotated(tilt, 1.0, 0.0, 0.0);

		monitorBody(w, h, d);	// 本体部分
		monitorScreen(sx, sy);	// スクリーン部分
	glPopMatrix();
}

// 仮想オブジェクトの設定
void object()
{
	// 物体色（反射係数等）の設定
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);	// 拡散反射係数
	glMaterialfv(GL_FRONT, GL_SPECULAR, gray);		// 鏡面反射係数
	glMateriali(GL_FRONT, GL_SHININESS, 10);		// ハイライトパラメータ

	// 回転する急須を表示
	glEnable(GL_LIGHTING);			// 陰影付け開始
	glPushMatrix();			// グループ化の開始
		glRotated(angle, 0.0, 1.0, 0.0);	// 物体を回転
		glTranslated(0.0, 0.0, 30.0);		// 物体を平行移動
		glutSolidTeapot(10.0);			// 物体を原点に配置
	glPopMatrix();			// グループ化の終了
}

// 仮想照明の設定
void light()
{
	static GLfloat pos[] = {0.0, 1.0, 0.5, 0.0};	// 照明光線の方向

	glEnable(GL_LIGHT0);			// 光源を有効化
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);	// 直接光の色を設定
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);	// 間接光の色を設定
	glLightfv(GL_LIGHT0, GL_POSITION, pos);	// 光源を配置
}

// 仮想カメラの設定
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

	camera();				// 仮想カメラを設定
	light();				// 仮想照明を設定
	object();				// 仮想物体を描画
	monitor();				// 仮想TVモニタを描画

	glutSwapBuffers();			// 画面を更新
}

// ウィンドウサイズ変更によるコールバック関数
// w, h：ウィンドウサイズ
void resize(int w, int h)
{
	glViewport(0, 0, w, h);	// ウィンドウ全体を表示領域に
	window_w = w;		// サイズを記憶
	window_h = h;
}

// タイマーによるコールバック関数
void anim(int data)
{
	scene();
	angle += 5.0;
	glutTimerFunc(timer, anim, 0);
}

// キー入力のコールバック関数
// key：キー文字
// x, y：マウスポインタの座標
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': fin(); break;
	case 'f': glutFullScreen(); break;
	case 'w': glutReshapeWindow(640, 480); break;
	}
	glutPostRedisplay();	// 再描画を要求
}

// 特殊キー入力のコールバック関数
void keypad(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: tilt -= 5.0; break;
	case GLUT_KEY_DOWN: tilt += 5.0; break;
	case GLUT_KEY_RIGHT: rot += 5.0; break;
	case GLUT_KEY_LEFT: rot -= 5.0; break;
	}
	glutPostRedisplay();	// 再描画を要求
}

int main(int argc, char *argv[])
{
	// 現実カメラの検出
	if (argc > 1) cam = *argv[1] - '0';
	if (!cap.open(cam)) fatal("キャプチャ開始に失敗");

	// ウィンドウの設定
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
				// カラー，隠面処理，裏画面を使う
	window = glutCreateWindow("OpenGL x OpenCV = Mixed Reality");
				// ウィンドウを生成（破棄のためウィンドウID も取得）
	glutReshapeWindow(window_w, window_h);

	// コールバックの設定
	glutReshapeFunc(resize);	// ウィンドウ変化関数の登録
	glutDisplayFunc(scene);		// シーン描画関数の登録
	glutKeyboardFunc(keybd);	// キー入力関数の登録
	glutSpecialFunc(keypad);	// 特殊キー入力関数の登録
	glutTimerFunc(timer, anim, 0);

	// レンダリングの設定
	glEnable(GL_DEPTH_TEST);	// 隠面処理
	glEnable(GL_NORMALIZE);		// 法線正規化

	// テクスチャの設定
	glGenTextures(10, tex);		// 一応 10個分を用意しておくけど...
		// ...実際に使用しているのは１個だけ

	// メインループ
	glutMainLoop();
	return (0);
}

