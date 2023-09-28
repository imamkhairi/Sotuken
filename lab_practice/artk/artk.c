/*
映像合成系 AR の最小限のプログラム artk.c 	Ver.2023.05.09
とりあえず，利用可能なマーカは１個だけ．
C99

■使用方法：
コンパイル：$ cc artk.c `pkgconf --cflags --libs artoolkit.pc` -o artk
実行：$ ./artk
操作：
	[Esc]／[Q]：終了
	[↑]／[↓]：物体の拡大・縮小

■AR（というか画像計測）に関する研究課題案：
カメラとマーカの両者が静止していても，正面方向から撮影する場合，
仮想映像に大きく激しい乱れが発生してしまう．
影響は，オブジェクトの奥行きサイズが大きい程，顕著に現れる．
一方，斜め方向からの撮影では，影響は少ない．
この不安定性（計測精度不良）の原因はカメラ映像の微小な乱れ（ジッタ）にある．

■練習問題案：
(1) 複数マーカへの対応など各自で改造しては？
（変数 marker, pid, pad, 等を配列化する必要があります．）

(2) AR を映像合成ではなく，センサとかコントローラとして利用してみては？
例えば，ARマーカをバイクハンドルに見立てて VR RCカーを運転できるように改造．
マーカの左右回転がステアリング，前後回転がアクセル／ブレーキとか．
*/

#include <AR/ar.h>	// ARTKの本体
#include <AR/video.h>	// ARTKのカメラ制御機能
#include <AR/param.h>	// ARTKのカメラ補正機能
#include <AR/gsub.h>	// ARTKとOpenGLの連携機能
#include <GL/gl.h>	// OpenGLの本体
#include <GL/glut.h>	// OpenGLアプリ開発用のフレームワーク
#include <GL/freeglut_ext.h>	// glutLeaveMainLoop()
#include <stdio.h>	// perror()

// カメラ関連の変数
char	*vconf = "-dev=/dev/video0";		// カメラドライバ設定データ
	// 複数カメラの切替には，文字列処理による番号変更が必要なので面倒...
		// この辺りについては，OpenCV を併用すればお手軽
char	*cparm = "Data/camera_para.dat";	// カメラ補正データファイル
ARUint8	*img;		// カメラ映像へのポインタ
int	w, h;		// カメラ映像のサイズ

// ARマーカ関連の変数
ARMarkerInfo	*marker = NULL;		// マーカ情報へのポインタ
int	pid = -1;			// マーカのパターンID
char	*pat = "Data/patt.hiro";	// マーカのパターンファイル
double	size = 80.0;			// マーカのサイズ(mm)
double	center[2] = {0.0, 0.0};		// マーカの原点位置(mm)
int	th = 127;			// マーカの二値化閾値

// 仮想物体関連の変数
GLfloat	black[4] = {0.0, 0.0, 0.0, 1.0};	// 黒 {r, g, b, 1}
GLfloat	gray[4] =  {0.5, 0.5, 0.5, 1.0};	// 灰 {r, g, b, 1}
GLfloat	white[4] = {1.0, 1.0, 1.0, 1.0};	// 白 {r, g, b, 1}
GLfloat	red[4] =   {1.0, 0.5, 0.5, 1.0};	// 赤 {r, g, b, 1}
GLfloat	green[4] = {0.5, 1.0, 0.5, 1.0};	// 赤 {r, g, b, 1}
GLfloat	blue[4] =  {0.5, 0.5, 1.0, 1.0};	// 青 {r, g, b, 1}
GLdouble	sz = 1.0;		// 拡大率

// 異常終了
void fatal(const char *msg)
{
	perror(msg);
	exit(1);
}

// カメラ映像表示
void drawReal()
{
	glDisable(GL_DEPTH_TEST);	// 隠面処理を解除
	argDrawMode2D();		// 2Dモードへ
	argDispImage(img, 0, 0);	// ウィンドウに画像を表示
}

// マーカ追跡
void trackMarker()
{
	ARMarkerInfo	*p;		// マーカ候補の配列へのポインタ
	int		n, i;		// マーカ候補の個数，カウンタ

	// マーカ候補の検出
	arDetectMarker(img, th, &p, &n);	// 候補を検出

	// マーカの特定
	marker = NULL;
	for (i = 0; i < n; i++, p++) {		// n個の候補から...
		if (p->id != pid) continue;		// パターンIDで絞り込み
		if (marker == NULL) marker = p;		// 最初の発見
		if (p->cf > marker->cf) marker = p;	// 信頼度で絞り込み
	}
}

// 仮想物体描画
void object()
{
	// 材質の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);	// 拡散反射成分
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);	// 環境反射成分
	glMaterialfv(GL_FRONT, GL_SPECULAR, red);	// 鏡面反射成分ツヤの色
	glMaterialf(GL_FRONT, GL_SHININESS, 30.0);	// ツヤの先鋭度

	// 座標変換：変換順序は記述順序の逆!!
	glPushMatrix();				// 変換状態を記憶
		glScaled(1.0, 1.0, sz);		// 4) 拡大
		glTranslated(0.0, 0.0, 30.0);	// 3) 床面上に移動
		glRotated(90.0, 1.0, 0.0, 0.0);	// 2) 横倒解消の回転
		glutSolidTeapot(40.0);		// 1) 急須を原点に配置
	glPopMatrix();				// 変換状態を回復
}

// 仮想照明設定
void light()
{
	// 照明光線の入射方向 {x, y, z, 0}
	static GLfloat	pos[4] = {0.0, 0.0, -100.0, 0.0};
		// カメラ座標系の場合... x：右，y：下，z：前
//	static GLfloat	pos[4] = {0.0, 0.0, 1.0, 0.0};
		// マーカ座標系の場合... x：右，y：上，z：表

	// 照明の設定
	glEnable(GL_LIGHTING);				// 陰影付けを有効化
	glEnable(GL_LIGHT0);				// 照明光源を有効化
	glLightfv(GL_LIGHT0, GL_POSITION, pos);		// 光源を配置
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white); 	// 照明の拡散光成分の色
	glLightfv(GL_LIGHT0, GL_AMBIENT, black); 	// 照明の環境光成分の色
	glLightfv(GL_LIGHT0, GL_SPECULAR, white); 	// 照明の鏡面光成分の色

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);	// 鏡面反射の計算を少しだけ高精度化
		// これがないと ARTK では光沢部分がかなり不安定...
			// さらなる高精度化にはプログラマブルシェーダが必要
}

// 仮想映像表示
void drawVirtual()
{
	static double	artm[3][4];	// カメラの変換行列（ARTK版）
	static double	gltm[16];	// カメラの変換行列（GL版）

	// CG描画の準備
	glEnable(GL_DEPTH_TEST);	// 隠面処理を有効化
	glEnable(GL_NORMALIZE);		// 方向ベクトルの正規化
	argDrawMode3D();		// 3Dモードへ
	argDraw3dCamera(0, 0);		// 表示先をウィンドウに
	glMatrixMode(GL_MODELVIEW);	// 仮想世界を開始

	// カメラ座標系への仮想物体の配置
	glLoadIdentity();	// カメラ座標系を開始
	light();		// 照明（カメラ基準で配置したい場合...）

	// マーカ座標系への仮想物体の配置
	if (marker) {		// マーカ検出済みの場合...
		arGetTransMat(marker, center, size, artm);	// 変換行列を取得
		argConvGlpara(artm, gltm);	// 変換行列を OpenGL形式へ変換
		glLoadMatrixd(gltm);		// マーカ座標系を開始
//		light();	// 照明（マーカ基準で配置したい場合...）
		object();	// 物体
	}
	/* artm の要素：カメラ座標系におけるマーカの方向・位置
		[0][0]，[0][1]，[0][2]：マーカのx軸の方向ベクトル
		[1][0]，[1][1]，[1][2]：マーカのy軸の方向ベクトル
		[2][0]，[2][1]，[2][2]：マーカのz軸の方向ベクトル
		[0][3]，[1][3]，[2][3]：マーカの中心の位置ベクトル
	ARTK の座標系：http://www.hitl.washington.edu/artoolkit/documentation/cs.htm */
}

// 映像処理（入力・合成・出力）
void video()
{
	// 映像入力
	img = (ARUint8 *)arVideoGetImage();	// カメラ映像を取得
	if (!img) { 				// 撮影未完了の場合...
		arUtilSleep(5);		// 時間つぶし（ミリ秒）
		return;			// 出直し
	}
	arVideoCapNext();			// 次フレームの撮影を開始

	// 映像合成
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// 画面をクリア
	drawReal();			// カメラ映像を表示
	trackMarker();			// マーカを追跡
	drawVirtual();			// 仮想映像を表示

	// アニメーションの処理も必要なら，この辺りに記述??

	// 映像出力
	argSwapBuffers();		// 画面を表示
}

// 通常キー入力
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case '\e' : case 'q':		// [Esc] or [Q] キーで終了
		glutLeaveMainLoop();		// freeglut_ext
		break;
	default: break;
	}
}
// 特殊キー入力
void keypad(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:	sz += 0.5; break;	
	case GLUT_KEY_DOWN:	sz = (sz < 1.0) ? 0.5 : (sz - 0.5); break;
	default: break;
	}
}

// マウス入力
void mouse(int button, int state, int x, int y)
{
	fprintf(stderr, "Button %d ", button);		// どのボタンが？
	switch (state) {
	case GLUT_DOWN: fprintf(stderr, "pressed.\n"); break;	// 押された？
	case GLUT_UP: fprintf(stderr, "released.\n"); break;	// 離された？
	default: break;
	}
}

// 開始
int main(int argc, char **argv)
{
	ARParam	cp, cp0;		// カメラの補正データ

	// フレームワークの準備
	glutInit(&argc, argv);		// GLUT を開始

	// マーカの準備
	if ((pid = arLoadPatt(pat)) < 0) fatal("パターン読込に失敗");

	// カメラの準備
	if (arVideoOpen(vconf) < 0) fatal("カメラ起動に失敗");
	if (arVideoInqSize(&w, &h) < 0) fatal("映像サイズ取得に失敗");
	if (arParamLoad(cparm, 1, &cp0) < 0) fatal("補正データ読込に失敗");
	arParamChangeSize(&cp0, w, h, &cp);	// 補正データを調整
	arInitCparam(&cp);			// 補正を有効化
	arVideoCapStart();			// 撮影を開始

	// ウィンドウの準備
	argInit(&cp, 1.0, 0, 0, 0, 0);		// ウィンドウを生成
	glutSetWindowTitle("AR sample");	// タイトルを設定

	// コールバックの設定
	glutSpecialFunc(keypad);		// 特殊キー入力
		// マウス，通常キー，ディスプレイについては argMainLoop() で設定

	// メインループ
	argMainLoop(NULL, keybd, video);	// ≒ glutMainLoop()
//	argMainLoop(mouse, keybd, video);	// 同上（マウス利用の場合）

	// 終了
	arVideoCapStop();	// 撮影を終了
	arVideoClose();		// カメラを停止
	argCleanup();		// ウィンドウを破棄
	return (0);
}

