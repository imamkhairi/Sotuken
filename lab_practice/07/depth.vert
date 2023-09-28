#version 140
// depth.vert : デプス（奥行）による彩色

out vec4	P;	// 頂点の位置ベクトルを補間してfrag側へ渡すよ
	// gl_Vertexとかに合わせて，vec3ではなくvec4(x,y,z,w)としてます．
	// vec3に変換してもOK．
	// z成分だけ out float pz とかでもOK．

void main(void)
{
	// 頂点のスクリーン座標
	gl_Position = ftransform();

	// 頂点の位置ベクトル
	P = gl_ModelViewMatrix*gl_Vertex;	// カメラ座標系に変換
}

