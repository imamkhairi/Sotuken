#version 140
// depth2.vert : デプス（奥行）による彩色の（多分）高速版

uniform float	arg = 1.0;	// アプリ側から数値が渡されて来るよ

out vec4	color;	// 頂点の色を補間してfrag側へ渡すよ

void main(void)
{
	// 頂点のスクリーン座標
	gl_Position = ftransform();

	// 頂点の位置ベクトル
	vec4	P = gl_ModelViewMatrix*gl_Vertex;	// カメラ座標系に変換

	// 頂点のデプス
	float	depth = (150.0 + P.z)/50.0;
		// 遠くを暗く，近くを明るくしてます
			// P.z=-150でdepth=0，P.z=-100でdepth=1としてます
		// 定数はアプリ側の物体設定に合わせて調整してね
			// カメラ座標系ではP.z < 0.0となることに注意

	// 頂点の色
	vec4	vc = gl_FrontMaterial.diffuse;

	color = vec4(vc.rgb*arg*depth, vc.a);
}

