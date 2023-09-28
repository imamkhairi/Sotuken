#version 140
// flat.frag
// フラットシェーディング：補間せず，ベタ塗り

flat in vec4	color;	// ピクセルの色がvert側から渡されて来るよ

void main(void)
{
	gl_FragColor = color;
}

