#version 140
// color2.vert : color の（多分）高速版
/*
多数のピクセル側でなく少数の頂点側で彩色．
全体的には大幅な高速化が見込まれる？
vert も frag も GPU 内で（完全ではありませんが）並列に処理されるので，
大差ない可能性もあります．
また，この例では単純すぎるので体感も不可能なんですが．
*/

uniform float	arg = 1.0;	// アプリ側から数値が渡されて来るよ

out vec4	color;	// 頂点の色を補間してfrag側へ渡すよ

void main(void)
{
	// 頂点のスクリーン座標（ピクセル位置）：vertの必須変数
//	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;	// 新しい記法
	gl_Position = ftransform();		// 古い記法

	// 基本色（RGBA値）の準備
	vec4	c = vec4(1.0, 0.5, 0.5, 1.0);	// シェーダ側での指定例
//	c = gl_FrontMaterial.diffuse;	// アプリ側の素材色の利用例

	// 頂点の色
	color = vec4(c.xyz*arg, c.a);
		// 引数argを輝度調整に利用してみた
}

