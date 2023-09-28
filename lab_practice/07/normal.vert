#version 140
// normal.vert
// 法線ベクトルによる彩色

out vec3	N;	// 頂点の法線ベクトルを補間してfrag側へ渡すよ

void main(void)
{
	gl_Position = ftransform();

	// 頂点の法線ベクトル
	N = normalize(gl_Normal);	// ローカル座標系のままの場合...
		// モデルとともに色彩も回転...塗装ですね
//	N = normalize(gl_NormalMatrix*gl_Normal);	// カメラ座標系に変換する場合...
		// モデルが回転しても色彩は空間に固定...投影マッピングですね
}

