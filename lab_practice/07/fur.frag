#version 140
// fur.frag
// 作成途中
// ファーシェーディング：起毛素材の簡易的な質感表現
// 人の頭髪のように「天使の輪」が見えるハズ
// バーテックスシェーダにはphong.vertを流用してね

in vec3	Pv, Nv;	// 方向ベクトルがvert側から渡されて来るよ
uniform float	arg;

void main(void)
{
	// 方向ベクトルの正規化
		// 頂点で正規化済みでも補間によって長さが変化してますよ
	vec3	N = normalize(Nv);			// 法線方向：物体→表側
	vec3	V = normalize(-Pv);			// 視線方向：物体→視点
	vec3	L = normalize(gl_LightSource[0].position.xyz);	// 入射方向：物体→光源

	// 各成分に利用するベクトル演算
	float	L_N = dot(L, N);
	float	V_N = dot(V, N);
	float	LxN = length(cross(L, N));
	float	VxN = length(cross(V, N));
	float	sd = sign(max(L_N, 0.0));	// 直接照明有無

	// 環境光成分
	vec4	A = gl_FrontLightProduct[0].ambient;

	// 拡散反射光成分：Lambertモデル
	float	d = sd*LxN;
	vec4	D = gl_FrontLightProduct[0].diffuse;

	// 鏡面反射光成分：Phongモデル
	float	R_V = sd*max(LxN*VxN - L_N*V_N, 0.0);
	float	s = pow(R_V, gl_FrontMaterial.shininess);
	vec4	S = gl_FrontLightProduct[0].specular;

	// 頂点の色
	gl_FragColor =	A + (D*d + S*s)*arg
		+ gl_FrontLightModelProduct.sceneColor;
}

