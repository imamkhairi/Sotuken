#version 140
// phong.vert
// Phong シェーディング：法線ベクトルを補間，ピクセル毎に彩色
// Goulaudよりかは高精度．Goulaudで目立っていたポリゴン境界は見られないハズ．
// 高コスト．frag側での処理増加によって全体の計算量が激増．

uniform float	arg = 0.0;	// 不使用

out vec3	Pv, Nv;	// 方向ベクトルを補間してfrag側へ渡すよ

void main(void)
{
	gl_Position = ftransform();

	// ベクトルの準備
	Pv = (gl_ModelViewMatrix*gl_Vertex).xyz;	// 位置
	Nv = gl_NormalMatrix*gl_Normal;			// 法線
		// どうせfrag側で必要なのでvert側でnormalize()は不要
}

