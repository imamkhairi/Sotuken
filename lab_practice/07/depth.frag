#version 140
// depth.frag : デプス（奥行）による彩色

uniform float	arg = 0.0;	// アプリ側から数値が渡されて来るよ

in vec4	P;	// 位置ベクトルがvert側から渡されて来るよ

void main(void)
{
	vec4	color = gl_FrontMaterial.diffuse;

	float	depth = (150.0 + P.z)/50.0;
		// 遠くを暗く，近くを明るくしてます
			// P.z=-150でdepth=0，P.z=-100でdepth=1としてます
		// 定数はアプリ側の物体設定に合わせて調整してね
			// カメラ座標系ではP.z < 0.0となることに注意

	gl_FragColor = vec4(color.rgb*arg*depth, color.a);
}

