#version 140
// phong.frag
// Phong シェーディング：法線ベクトルを補間，ピクセル毎に彩色
// Goulaudよりかは高精度．Goulaudで目立っていたポリゴン境界は見られない．
// 高コスト．frag側での処理増加によって全体の計算量が激増．

in vec3	Pv, Nv;	// 方向ベクトルがvert側から渡されて来るよ

void main(void)
{
	// 方向ベクトルの正規化
		// 頂点で正規化済みでも補間によって長さが変化してますよ
	vec3	N = normalize(Nv);			// 法線方向：物体→表側
	vec3	V = normalize(-Pv);			// 視線方向：物体→視点
	vec3	L = normalize(gl_LightSource[0].position.xyz);	// 入射方向：物体→光源

	// 環境光成分
	vec4	A = gl_FrontLightProduct[0].ambient;

	// 拡散反射光成分：Lambertモデル
	float	d = max(dot(L, N), 0.0);
	vec4	D = gl_FrontLightProduct[0].diffuse;
/*
	// 鏡面反射光成分：Phongモデル
	vec3	R = -normalize(reflect(L, N));
	float	s = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);
*/
	// 鏡面反射光成分：Blinによる近似モデル
	vec3	H = normalize(V + L);
	float	s = pow(max(dot(H, N), 0.0), gl_FrontMaterial.shininess);

	vec4	S = gl_FrontLightProduct[0].specular;

	// 頂点の色
	gl_FragColor =	A + D*d + S*s*sign(d)
		+ gl_FrontLightModelProduct.sceneColor;
}

