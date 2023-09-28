#version 140
// normal.frag
// 法線ベクトルによる彩色

uniform float	arg = 0.0;	// アプリ側から数値が渡されて来るよ

in vec3	N;	// 法線ベクトルがvert側から渡されて来るよ

void main(void)
{
	// 法線ベクトルの xyz を rgb に変換
	vec3	rgb = N + vec3(1.0, 1.0, 1.0);

	gl_FragColor = vec4(rgb*arg, 1.0);
}

