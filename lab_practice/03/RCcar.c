/*
GLUT のサンプルプログラム RCcar.cc：VR?ラジコンカー
コンパイル：$ cc RCcar.c -lglut -lGLU -lGL -lm -o RCcar
実行：$ ./RCcar
操作：[H/←][L/→]でステアリング，
	[Space/↑]でアクセル，[B/↓]でブレーキ，
	[R]でリセット，[Q]で終了
*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	GLdouble	x, y, v;	// 座標，速度
	GLdouble	dir, str;	// 方位角，舵角
	GLfloat		*color;		// 色
} Car;
Car	car;		// 自動車

GLfloat	lt_p[4] = {0.0, 0.0, 2500.0, 1.0};	// 照明光源の位置 {x, y, z, 1}
GLfloat	black[4] = {0.0, 0.0, 0.0, 1.0};	// 黒
GLfloat	white[4] = {1.0, 1.0, 1.0, 1.0};	// 白
GLfloat	gray[4] = {0.5, 0.5, 0.5, 1.0};		// 灰
GLfloat	red[4] = {1.0, 0.0, 0.0, 1.0};		// 赤
GLfloat	camera[16];		// カメラ行列
int interval = 30;		// 画面更新の周期 [msec]


// 自動車を初期化
void setCarPos(Car *p, GLdouble x, GLdouble y)
{
	p->x = x;
	p->y = y;
	p->v = 0.0;
}
void setCarDir(Car *p, GLdouble dir)
{
	p->dir = dir;
	p->str = 0.0;
}
void paintCar(Car *p, GLfloat *c)
{
	p->color = c;
}
void resetCar(Car *p)
{
	setCarPos(p, 0.0, 0.0);
	setCarDir(p, 0.0);
}

// 自動車を操縦
void moveCar(Car *p)
{
	if (p->v > 30.0) p->v = 30.0;		// 速度を制限
	if (p->v < 0.0) p->v = 0.0;	// 停止
//	if (p->v < -30.0) p->v = 30.0;	// 後退
	p->x += p->v*cos(p->dir*M_PI/180.0);	// 移動
	p->y += p->v*sin(p->dir*M_PI/180.0);
	if (p->v != 0.0) p->dir += p->str;	// 旋回
	p->str = 0.0;				// 舵角をリセット
}
void steerCar(Car *p, double s)
{
	p->str = s;
}
void accCar(Car *p, double a)
{
	p->v += a;
}

// キー入力によるコールバック関数
// key：キー文字
// x, y：マウスポインタの座標
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'h': steerCar(&car, 5.0); break;	// 左操舵
	case 'l': steerCar(&car, -5.0); break;	// 右操舵
	case ' ': accCar(&car, 5.0); break;	// 加速・前進
	case 'b': accCar(&car, -5.0); break;	// 減速・後退
	case 'r': resetCar(&car); break;	// リセット
	case 'q': exit(0); break;	// 終了
	default: break;
	}
	glutPostRedisplay();
}

// 特殊キー入力のコールバック関数
// key：キーコード
// x, y：マウスポインタの座標
void keypad(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT :	steerCar(&car, 5.0); break;	// 左操舵
	case GLUT_KEY_RIGHT :	steerCar(&car, -5.0); break;	// 右操舵
	case GLUT_KEY_UP:	accCar(&car, 5.0); break;	// 加速・前進
	case GLUT_KEY_DOWN:	accCar(&car, -5.0); break;	// 減速・後退
	default: break;
	}
	glutPostRedisplay();
}

// 自動車を描画
void drawCar(Car *p)
{
	// 物体色を設定
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, p->color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 10);

	// 座標変換
	glPushMatrix();		// このブロック内，処理順序は記述順序の逆!!
		// 移動後の位置・姿勢
		glTranslated(p->x, p->y, 0.0);		// 5) 位置を設定
		glRotated(p->dir, 0.0, 0.0, 1.0);	// 4) 方向を設定

		// 移動前の位置・姿勢
		glTranslated(0.0, 0.0, 50.0);		// 3) 底面を床面に
		glRotated(90.0, 1.0, 0.0, 0.0);		// 2) x-y平面を基準面に
		glutSolidTeapot(50.0);			// 1) 原点に配置
			// 50.0 -> ukurannya emang jadi 50
	glPopMatrix();
}

// CG を描画
void render()
{
	glEnable(GL_DEPTH_TEST);		// 隠面処理を開始
	glClear(GL_DEPTH_BUFFER_BIT);		// デプスバッファを消去
	glMatrixMode(GL_MODELVIEW);		// モデリングを開始
	glLoadMatrixf(camera);			// カメラを配置
		// replace the current matrix with an arbitrary matrix.
	glEnable(GL_LIGHTING);			// 陰影付け開始
	glEnable(GL_LIGHT0);			// 光源を有効化
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);	// 光源の色を設定
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);	//     〃
	glLightfv(GL_LIGHT0, GL_POSITION, lt_p);// 光源を配置
	drawCar(&car);				// 自動車を描画
}

// 床のタイルを描画
// x, y：中心の座標	r：半辺長
void drawTile(double x, double y, double r)
{
	glPushMatrix();
		glTranslated(x*r*2.0, y*r*2.0, 0.0);
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3d(-r,  r, 0.0);	// 4頂点→四角形
			glVertex3d(-r, -r, 0.0);
			glVertex3d( r,  r, 0.0);
			glVertex3d( r, -r, 0.0);
		glEnd();
	glPopMatrix();
}

// 床の全体を描画
// w, h：タイルの枚数
void drawFloor(int w, int h)
{
	GLfloat *c[2] = { white, gray };
	int x, y;

	glMaterialfv(GL_FRONT, GL_SPECULAR, black);	// ツヤ無しに
	glTranslated(-(w-1)*150.0, -(h-1)*150.0, 0.0);	// 中心位置を調整
	for (y = 0; y < h; y++)
	for (x = 0; x < w; x++) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
			c[(x+y)%2]);	// 市松模様に色を設定
		drawTile(x, y, 150.0);	// タイル
	}
}

// 描画要求 glPostRedisplay() 等によるコールバック関数
void disp()
{
	glClear(GL_COLOR_BUFFER_BIT);		// カラーバッファを消去
	render();
	drawFloor(5, 5);			// 床を描画
	glutSwapBuffers();			// 画面を表示
}

// ウィンドウサイズ変更によるコールバック関数
// w, h：ウィンドウサイズ
void resize(int w, int h)
{
	glViewport(0, 0, w, h);			// ウィンドウ全体を表示領域に
	glMatrixMode(GL_PROJECTION);		// 投影法の設定を開始
	glLoadIdentity();
	gluPerspective(30.0, (double)w/(double)h, 1.0, 10000.0);
}

// タイマーによるコールバック関数，自動車の運動
// data：glutTimerFunc() の第3引数
void update(int data)
{
	moveCar(&car);				// 自動車の運動
	glutPostRedisplay();			// 再描画
	glutTimerFunc(interval, update, 0);	// タイマー再設定
}

int main(int argc, char **argv)
{
	// ウィンドウの設定
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutCreateWindow("RC car");		// ウィンドウ作成
	glutReshapeWindow(800, 600);		// ウィンドウサイズ変更
	glClearColor(0.0, 1.0, 1.0, 1.0);	// 背景色設定

	// コールバックの設定
	glutDisplayFunc(disp);
	glutKeyboardFunc(keybd);
	glutSpecialFunc(keypad);
	glutReshapeFunc(resize);
	glutTimerFunc(interval, update, 0);

	// カメラの設定
	glMatrixMode(GL_MODELVIEW);			// モデリングを開始
	glLoadIdentity();				// 変換行列をリセット
	gluLookAt(0.0, -2500.0, 500.0, 0.0, 0.0, 10.0,
		0.0, 1.0, 0.0);				// カメラアングルを設定
	glGetFloatv(GL_MODELVIEW_MATRIX, camera);	// 変換行列を保存

	// 自動車の初期化
	resetCar(&car);
	paintCar(&car, red);

	// メインループ
	glutMainLoop();			// ラジコン開始
	return (0);
}

