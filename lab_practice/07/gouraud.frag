#version 140
// gouraud.frag
// Gouraud シェーディング：頂点毎に彩色，ピクセル毎に補間
// OpenGL固定シェーダのクローン

in vec4	color;	// ピクセルの色がvert側から渡されて来るよ

void main(void)
{
	gl_FragColor = color;
}

