/*
GLSL テストプログラム glsl.c
C99
コンパイル：$ cc glsl.cc -lglut -lGLU -lGL -o glsl
実行：$ ./glsl シェーダ &
操作：[Q] で終了，[F] で全画面化，[W] でウィンドウ化，
	[↑/↓/←/→] で回転，[Spc/B] で前後移動，
	[O] で物体変更，[H/L] で uniform float 変数値の増減
*/

#define GL_GLEXT_PROTOTYPES	// OpenGLの拡張機能
#include <GL/gl.h>		// OpenGLの標準機能
#include <GL/glext.h>		// OpenGLの拡張機能

#include <GL/glu.h>

#include <GL/glut.h>		// GLUTの標準機能
#include <GL/freeglut_ext.h>	// freeglutの拡張機能

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ウィンドウ関連
#define	WIN_W	640
#define	WIN_H	480
int	scr_w, scr_h;	// 現在の画面サイズ（全画面とウィンドウの両モード共通）
int	win;		// ウィンドウID

// モデリング関連
int	obj = 0;	// 物体の種類
double	z = 0.0;
double	tilt = 0.0;
double	rot = 0.0;
GLfloat red[] = {1.0, 0.5, 0.5, 1.0};
GLfloat green[] = {0.5, 1.0, 0.5, 1.0};
GLfloat blue[] = {0.5, 0.5, 1.0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
GLfloat black[] = {0.0, 0.0, 0.0, 1.0};

// シェーダ関連
GLuint	prg = 0;	// シェーダプログラムID
	// 複数化して，複数のシェーダを使い分けてもよい．
GLfloat	uVal = 1.0;	// GLSLのuniform float変数の値
	// 配列化して，uniform vec4 変数とかにしてもよい．
GLint	uLoc;		// GLSLのuniform float変数のロケーション
	// GLSLの変数へのポインタみたいな物です．

// 異常終了
void fatal(const char *msg)
{
	perror(msg);
	exit(1);
}

// 警告表示
void debug(const char *msg)
{
	fputs(msg, stderr);
}

// エラーチェック
int chkErr(const char *msg)
{
	GLenum	e = glGetError();
	if (e == GL_NO_ERROR) return (0);
	fprintf(stderr, "%s : 0x%04X\n", msg, e);
	return (1);
}

// 物体
void object(double x, double y, double z)
{
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red);
	glMaterialf(GL_FRONT, GL_SHININESS, 30.0);

	glPushMatrix();
		glTranslated(x, y, z);
		glRotated(rot, 0.0, 1.0, 0.0);
		glRotated(tilt, 1.0, 0.0, 0.0);
		switch (obj%3) {
		case 0: glutSolidTeapot(10.0); break;
		case 1: glutSolidSphere(10.0, 20, 20); break;
		case 2: glutSolidCube(20.0); break;
		}
	glPopMatrix();
}

// カメラ
void camera()
{
	static double	fovy = 30.0;	// y軸方向の視野角
	static double	near = 1.0;	// 視界の最近端までの距離
	static double	far = 10000.0;	// 視界の最遠端までの距離

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)scr_w/(double)scr_h, near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 100.0,	// カメラの位置
		0.0, 0.0, 0.0,		// 注視点の位置
		0.0, 1.0, 0.0);		// カメラの上方向
	/* カメラ座標系（GL_MODELVIEW）への変換後には，
		カメラ位置が原点，右が+x，上が+y，後方が+zとなる．
		前方は-zであることに注意． */
}

// 光源
void light()
{
	static GLfloat pos[] = {0.0, 1.0, 1.0, 0.0};	// 照明光線の方向（カメラ座標系）

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gray);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
}

// シーン：描画要求 glPostRedisplay() 等によるコールバック関数
void scene()
{
	glClearColor(0.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	camera();
	light();

	// 固定シェーダ利用による物体描画
	glUseProgram(0);
	object(-15.0, 0.0, z);	

	// プログラマブルシェーダ利用による物体描画
	glUseProgram(prg); chkErr("glUseProgram() 失敗");
		// uniform変数の有無・名前とかGLSL側との矛盾があると失敗しますよ．
	glUniform1f(uLoc, uVal);	// uniform変数も利用
	object(+15.0, 0.0, z);		// 物体を描画

	glutSwapBuffers();
}

// ウィンドウサイズ変更によるコールバック関数
void resize(int w, int h)
{
	glViewport(0, 0, w, h);	// ウィンドウ全体を表示領域に
	scr_w = w;	// 現在の画面サイズを更新
	scr_h = h;
}

// キー入力のコールバック関数
void keybd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q': glutLeaveMainLoop(); break;		// freeglut_ext.h
	case 'f': glutFullScreenToggle(); break;	// freeglut_ext.h
	case 'w': glutLeaveFullScreen(); break;		// freeglut_ext.h
	case ' ': z += 10.0; break;
	case 'b': z -= 10.0; break;
	case 'o': obj++; break;
	case 'h': uVal += 0.1; break;
	case 'l': uVal -= 0.1; if (uVal < 0.0) uVal = 0.0; break;
	}
	glutPostRedisplay();
	// mark current window to redisplay
}

// 特殊キー入力のコールバック関数
void keypad(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:	tilt -= 5.0; break;
	case GLUT_KEY_DOWN:	tilt += 5.0; break;
	case GLUT_KEY_RIGHT:	rot += 5.0; break;
	case GLUT_KEY_LEFT:	rot -= 5.0; break;
	}
	glutPostRedisplay();
}

//////// シェーダソース＝テキストファイル用の文字列処理関数群 ////////

// 文字列の末尾部分の比較
int strCmpTail(const char *s, const char *t)
{
	int	ns = strlen(s);
	int	nt = strlen(t);
	if (ns < nt) return (-1);
	s = s + ns - nt;
	return (strcmp(s, t));
	// strcmp return 0 if equal
}

// 文字列の連結
// 連結結果の使用後には free() すべき．
char *strCat(const char *s1, const char *s2)
{
	int	size = strlen(s1) + strlen(s2) + 1;	// +1 for '\0'
	char	*buf = (char *)malloc(sizeof(char)*size);
	if (!buf) return (NULL);
	strncpy(buf, s1, size);
	// copy string to buf from s1 with size of (size)
	strncat(buf, s2, size);
	// append to buf from s2 with length of size
	return (buf);
}

// テキストファイル内の全文字列の取得：ファイルポインタ版
// 文字列の使用後には free() すべき．
char *fReadText(FILE *fp)
{
	if (fseek(fp, 0, SEEK_END)) return (NULL);
	// fseek return 0 if successful

	long	len = ftell(fp); if (len < 1) return (NULL);
	// returns the current file position of the specified stream with respect to the starting of the file. 
	// This function is used to get the total size of file after moving the file pointer at the end of the file
	// jadi kalau < 1 artinya kosong kan

	char	*buf = (char *)malloc(sizeof(char)*(len + 1));	// +1 for '\0'
	// malloc only allocate the memory 
	// calloc allocate and initialize the value to 0

	if (!buf) return (NULL);
	// if the memory allocation is failed

	rewind(fp);
	char	*s = buf;
	int	c;
	while (c = fgetc(fp)) {
		if (c == EOF) break;
		*s = c;
		s++;
	}
	*s = '\0';
	return (buf);
}

// テキストファイル内の全文字列の取得：ファイル名版
// 文字列の使用後には free() すべき．
char *getText(const char *fn)
{
	FILE	*fp = fopen(fn, "r"); if (!fp) return (NULL);
	char	*s = fReadText(fp);
	fclose(fp);
	return (s);
}

//////// シェーダ組込用の関数群 ////////

// シェーダ単体の組込
int loadShader(GLuint prg, GLenum type, const char *fn)
{
	// loadShader(prg, type, fn[i])
	char	*src = getText(fn);	// シェーダソース文字列の取得
	if (!src) return (1);
	// kalau src = NULL return
	// kan kalau !NULL artinya not NULL jadinya if berlaku

	GLuint	sh = glCreateShader(type);
	// creates an empty shader object and returns a non-zero value by which it can be referenced

	if (!sh) { free(src); return (1); }

	fprintf(stderr, "\t%s : ", fn);
	glShaderSource(sh, 1, (const GLchar **)&src, NULL);
	free(src);

	glCompileShader(sh);
	//  compiles the source code strings that have been stored in the shader object specified by shader.
	// This value will be set to GL_TRUE if the shader was compiled without errors, and GL_FALSE otherwise.

	GLint	stts;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &stts);
	// return a parameter from a shader object

	if (stts == GL_FALSE) {
		debug("コンパイルエラー：\n");
		GLsizei	buflen, debuglen;
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &buflen);

		GLchar	buf[buflen];
		glGetShaderInfoLog(sh, buflen, &debuglen, buf);
		debug(buf);
		return (1);
	}

	debug("OK.\n");

	glAttachShader(prg, sh);
	// Attaches a shader object to a program object

	glDeleteShader(sh);
	// Delete a shader object

	return (0);
}

// シェーダのファイルとタイプの対応関係
typedef struct {
	const char	*ext;	// ファイル拡張子
	GLenum		type;	// シェーダタイプ
} ShaderTab;

ShaderTab	shadertab[] = {
	{".vert", GL_VERTEX_SHADER},
	{".geom", GL_GEOMETRY_SHADER},
	{".frag", GL_FRAGMENT_SHADER},
	{NULL, 0}
};

// シェーダタイプの特定
GLenum shaderType(const char *fn)
{
	ShaderTab	*p = shadertab;
	while (p->type) {
		if (strCmpTail(fn, p->ext) == 0) return (p->type);
		p++;
	}
	return (0);
}

// シェーダ群の一括組込：ファイル名ベース版
	  // if (!loadShaderGrp(prg, fn[i])) e++;
int loadShaderGrp(GLuint prg, const char *bn)
{
	ShaderTab	*p = shadertab;
	int	n = 0;

	while (p->type) {
		char	*fn = strCat(bn, p->ext);	// fn = "base" + ".ext"
		if (!fn) fatal("strCat() 失敗");
		if (!loadShader(prg, p->type, fn)) n++;
		free(fn);
		p++;
	}
	return (n);	// 組込成功したシェーダの個数
}

// シェーダ群の一括組込
int loadShaders(GLuint prg, char *fn[], int n)
{
	// *fn[] nama filenya, n argc ada berapa
	int	e = 0;

	debug("シェーダのコンパイル：\n");
	for (int i = 0; i < n; i++) {
		GLenum	type = shaderType(fn[i]);
		// shaderType() akan return 0 kalau file extensionnya salah

		// printf("%s\n", fn[i]);
		// fn[i] -> nama file shadernya
		if (!type) {
			if (!loadShaderGrp(prg, fn[i])) e++;
		} else {
			if (loadShader(prg, type, fn[i])) e++;
		}
	}
	return (e);	// 組込失敗したシェーダの個数
}

// シェーダプログラムの準備
GLuint initProgram(char *fn[], int n)
{
	GLuint	prg = glCreateProgram();
	if (!prg) { debug("glCreateProgram() 失敗\n"); return (0); }

	// prg = initProgram(argv, argc);

	if (loadShaders(prg, fn, n)) { debug("loadShaders() 失敗\n"); return (0); }
	// ga jalan kalau return dari loadShaders = 0

	glLinkProgram(prg); if (chkErr("glLinkProgram() 失敗")) return (0);

	GLint	stts;

	glGetProgramiv(prg, GL_LINK_STATUS, &stts);
	// return a parameter from a program object

	if (stts == GL_FALSE) {
		debug("リンクエラー：\n");
		GLsizei	buflen, debuglen;
		glGetProgramiv(prg, GL_INFO_LOG_LENGTH, &buflen);
		GLchar	buf[buflen];
		glGetProgramInfoLog(prg, buflen, &debuglen, buf);
		debug(buf);
		return (0);
	}
	return (prg);
}

// シェーダプログラムの終了
void finProgram(GLuint prg)
{
	glDeleteProgram(prg); 
	// delete a program object
	chkErr("glDeleteProgram() 失敗");
}

//////// メイン ////////
int main(int argc, char *argv[])
{
	// ウィンドウの設定
	glutInit(&argc, argv);
	if (argc < 2) fatal("シェーダソースファイル名の指定が必要");
	argv++; argc--;
	// argc-- karena dihitung dari ./file makanya untuk nama file shadernya -1 dari argc
	// argv++ biar pointernya udh ga di ./file lagi
	// argc start 0
	// argv ./execfile

	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	win = glutCreateWindow("GLSL test");
	glutReshapeWindow(WIN_W, WIN_H);

	// シェーダの設定
	prg = initProgram(argv, argc);
	if (!prg) fatal("initProgram() 失敗");

	// シェーダ引数の設定
	GLuint	uLoc = glGetUniformLocation(prg, "arg");
		// シェーダソース内 uniform float arg; とかの場合
		// Returns the location of a uniform variable
		// returns an integer that represents the location of a specific uniform variable within a program object.
		// return -1 if name ("arg") is does not correspond with any active variable
	if (uLoc < 0) fatal("glGetUniformLocation(arg) 失敗");

	// コールバックの設定
	glutReshapeFunc(resize);
	// reshape callback for the current window. 
	// The reshape callback is triggered when a window is reshaped. 
	// A reshape callback is also triggered immediately before a window's first display callback 
	// after a window is created or whenever an overlay for the window is established.
	
	glutDisplayFunc(scene);
	
	glutKeyboardFunc(keybd);
	
	glutSpecialFunc(keypad);
	// glutSpecialFunc sets the special keyboard callback for the current window. 

	// レンダリングの設定
	glEnable(GL_DEPTH_TEST);	// 隠面処理
	glEnable(GL_NORMALIZE);		// 法線正規化

	// テクスチャの設定
	// glGenTextures(10, tex);

	// メインループ
	glutMainLoop();

	// 終了
	finProgram(prg);
	glutDestroyWindow(win);
	return (0);
}