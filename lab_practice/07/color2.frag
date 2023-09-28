#version 140
// color2.frag : color の（多分）高速版
/*
多数のピクセル側でなく少数の頂点側で彩色．
全体的には大幅な高速化が見込まれる？
vert も frag も GPU 内で（完全ではありませんが）並列に処理されるので，
大差ない可能性もあります．
また，この例では単純すぎるので体感も不可能なんですが．
*/

in vec4	color;	// ピクセルの色がvert側から渡されて来るよ

void main(void)
{
	// フラグメント（ポリゴン内のピクセル）の色（RGBA値）：fragの必須変数
	gl_FragColor = color;
}

