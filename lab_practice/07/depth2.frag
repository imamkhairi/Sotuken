#version 140
// depth2.frag : デプス（奥行）による彩色の（多分）高速版

in vec4	color;	// 色がvert側から渡されて来るよ

void main(void)
{
	gl_FragColor = color;
}

