#version 140
// color.vert : 単色の塗り絵の頂点シェーダ
/*
out変数（フラグメントシェーダへ渡す変数）が皆無なので，
この頂点シェーダの存在価値も無いが，使っても問題は無い．
フラグメントシェーダだけでも実行可能．
*/

void main(void)
{
	// 頂点のスクリーン座標（ピクセル位置）：vertの必須変数
//	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;	// 新しい記法
	gl_Position = ftransform();		// 古い記法
}

