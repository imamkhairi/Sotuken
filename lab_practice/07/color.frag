#version 140
// color.frag : 単色の塗り絵のフラグメントシェーダ
// インタラクティブに輝度調整可能

uniform float	arg = 0.0;	// アプリ側から数値が渡されて来るよ

void main(void)
{
	// 基本色（RGBA値）の準備
	vec4	c = vec4(1.0, 0.5, 0.5, 1.0);	// シェーダ側での指定例
	//vec4	c = gl_FrontMaterial.diffuse;	// アプリ側の素材色の利用例

	// フラグメント（ポリゴン内のピクセル）の色（RGBA値）：fragの必須変数
	gl_FragColor = vec4(c.xyz*arg, c.a);
		// 引数argを輝度調整に利用してみた
}

