/*
GLUT の基本コード glut-1.c
	3D-CG の表示... カメラ，照明，物体の設定を理解しよう
コンパイル：$ cc glut-1.c -lGL -lGLU -lglut -o glut-1
実行：$ ./glut-1
操作：[Q] で終了，[H/←]と[L/→] で急須を回転
*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

int window_w = 640;		// ウィンドウの幅
int window_h = 480;		// ウィンドウの高さ
double angle = 0.0;		// 急須の回転角度

// オブジェクトの設定
void object()
{
	static GLfloat diff[] = {0.8, 0.4, 0.4, 1.0};	// 拡散反射係数 RGBA
		// diffuse reflection coefficient
	static GLfloat spec[] = {0.4, 0.4, 0.4, 1.0};	// 鏡面反射係数 RGBA
		// specular refrelction coefficient
	static GLint hl = 10;			// ハイライトパラメータ
		// highlight parameter [0, 128]

	// 物体色（反射係数等）の設定
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, hl);	
		// glmaterialfv function specifies material parameters for the lighting model
		

	// 物体の配置（合成変換を逆順に指定すること!!）
	// 回転する急須
	glPushMatrix();			// グループ化の開始
		// push matrix to the current stack
		// start a matrix stack? (since there isn't)
		glRotated(angle, 1.0, 1.0, 0.0);	// 物体を回転
			// angle is current angle
			// all three axis value will be normalize so that it have a direction with 1.0 size
		glTranslated(0.0, 0.0, 30.0);		// 物体を平行移動
		glutSolidTeapot(10.0);			// 物体を原点に配置
			// 			size
			// render a solid or wireframe teapot
	glPopMatrix();			// グループ化の終了

	// 四角柱
	glPushMatrix();
		glScaled(10.0, 30.0, 10.0);		// 物体を拡大
			// 　x,  y,  z
			// scale the object
		glutSolidCube(1.0);			// 物体を原点に配置
	glPopMatrix();
	/* glPushMatrix() 〜 glPopMatrix() が
		変換の対象範囲をグループ化していますよ．
		これらを書き忘れるとどうなるか？試してみては？ */

	/* 数学的に，物体やカメラの座標はベクトルとして表現され，
		移動・回転・拡大などの座標変換には行列が利用されています．
		変換行列とベクトル（変換前の座標）との積が
		新しいベクトル（変換後の座標）となります． */
}

// 照明の設定
void light()
{
	static GLfloat col[] = {1.0, 1.0, 1.0, 1.0};	// 照明光の色 RGBA
	static GLfloat pos[] = {0.0, 1.0, 0.5, 0.0};	// 照明光線の方向
		// using glfloat istead of float makes this does not depends on the system (bit)


	glEnable(GL_LIGHTING);			// 陰影付け開始
	glEnable(GL_LIGHT0);			// 光源を有効化
		// glEnable -> enable or disable OpenGL capabalities
		
		// GL_LIGHTING 
		// If enabled, use the current lighting parameters to compute the vertex color or index. 
		// If disabled, associate the current color or index with each vertex. 

		// GL_LIGHTi
		// If enabled, include light i in the evaluation of the lighting equation.

		// more option refer to documentation
		// 

	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);	// 直接光の色を設定
	glLightfv(GL_LIGHT0, GL_AMBIENT, col);	// 間接光の色を設定
	glLightfv(GL_LIGHT0, GL_SPECULAR, col);	// 間接光の色を設定
	glLightfv(GL_LIGHT0, GL_POSITION, pos);	// 光源を配置
		// 		GLenum light, GLenum pname, GLfloat *params
		// 
		// BRDF considers light from incoming direction and how much its reflected of a surface at certain point
		// Ambient Lighting is a light that reflected of other object onto what we are looking at.
		// Specular Lighting is light hits an object surface and reflects back toward the camera. (light source reflection in object)
		// Diffuese Lighting is light that comes from light source but not directly reflected to the viewing point. (that is why this is not as brigth as specular light)
		// Emissive lighting is ligth that emitted by an object such as a light bulb,\.

}

// カメラの設定
void camera()
{
	double	fovy = 30.0;		// y軸方向の視野角
	double	near = 1.0;		// 視界の最近端までの距離
	double	far = 10000.0;		// 視界の最遠端までの距離

	// カメラの視野の設定
	glMatrixMode(GL_PROJECTION);	// 透視投影を開始
		// 		mode(prespective projection)
		// GL_PROJECTION used to create viewing volume
		// set up what we want to see from what we created
		// specifies which matix is the current matrix
	glLoadIdentity();		// 変換行列をリセット
		// replace current matrix with the identity matrix
	gluPerspective(fovy, (double)window_w/(double)window_h, near, far);
		// 	   	   fovy, aspect, zNear, zFar
		// fovy   -> how the field angle, in degrees, in the y direction
		// aspect -> specifies the aspect ratio that determines the fov in the x direction. 
		// 			 the aspect ratio is the ratio of x(width) to y(height)
		// zNear  -> Specifies the distance from the viewer to the near clipping plane(always positive).
		// zFar   -> Specifies the distance from the viewer to the far clipping plane (always positive)
					// 投影パラメータを設定
		// set up a projection matrix
		// 
		// refer to opengl rendering 
		// prespective projection

	// カメラの位置の設定
	glMatrixMode(GL_MODELVIEW);	// モデリングを開始
		// 		 mode
		//  GL_MODELVIEW used to make various transformation to the models
	glLoadIdentity();		// 変換行列をリセット
	gluLookAt(0.0, 30.0, 100.0,	// カメラの位置
			  0.0, 0.0, 0.0,		// 注視点の位置
			  0.0, 1.0, 0.0);		// カメラの上方向
		// define a viewing transformation
		// eyex, eyey, eyez -> view coordinate (point) (camera)
			// determine camera coordinate
		// centerX, centerY, cewnterZ -> position of the reference point
			// point indicationg the center of the scene (where the camera looking)
		// upX, upY, upZ -> specifies the direction of the up vector
			// upVector controls where the camera look at. (90 degree with view at point)
			// upVector pointing from the camera to its above (up), imagine this as a normal vector
			// relative to the camrea not to the world

		// horizontal plane is xz plane
		// vertical determined by y value
}

// 描画要求 glPostRedisplay() 等によるコールバック関数
void scene()
{
	glClearColor(0.0, 1.0, 1.0, 1.0);	// 背景色設定
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		// 	mask -> indicate the buffers to be cleared
				// カラーバッファとデプスバッファを消去
		// clear buffers to preset values

	// work as a canvas, so other object that wanted to show have to included here

	camera();				// カメラを設定
	light();				// 照明を設定
	object();				// 物体を設定

	glutSwapBuffers();			// 画面を更新
		// swaps the buffers of the current window if double buffered.
}

// キー入力のコールバック関数
// key：キー文字
// x, y：マウスポインタの座標
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': exit(0); break;

	case 'h': angle -= 5.0; break;
	case 'l': angle += 5.0; break;
	}
	glutPostRedisplay();
}

// 特殊キー入力のコールバック関数
// key：キーコード
// x, y：マウスポインタの座標
void keypad(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT : angle -= 5.0; break;
	case GLUT_KEY_RIGHT : angle += 5.0; break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	// ウィンドウの設定
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
				// カラー，隠面処理，裏画面を使う
				// GLUT_DEPTH Bit mask to select a window with a depth buffer. so that having depth
				// GLUT_DOUBLE to make it become double buffer

	glutCreateWindow("GLUT: 3DCG");
	glutReshapeWindow(window_w, window_h);

	// コールバックの設定
	glutDisplayFunc(scene);		// シーン描画関数の登録
		// sets the display callback for the current window.
	glutKeyboardFunc(keybd);	// キー入力関数の登録
		// sets the keyboard callback for the current window.
	glutSpecialFunc(keypad);	// 特殊キー入力関数の登録
		// sets the special keyboard callback for the current window.
		// i.e. function keyboard and arrow or else

	// レンダリングの設定
	glEnable(GL_DEPTH_TEST);	// 隠面処理
		// to enable depth_test
		// depth buffer use to prevent triangles rendering in the front while they're supposed to be behind the orher triangles
	glEnable(GL_NORMALIZE);		// 法線正規化
		// if enabled normal vectors are normalized to unit length after transformation and before lighting. 

	// メインループ
	glutMainLoop();		// 表示等の開始
	return (0);
}

