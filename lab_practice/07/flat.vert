#version 140
// flat.vert
// フラットシェーディング：補間せず，ベタ塗り

uniform float	arg = 0.0;	// 不使用

flat out vec4	color;	// 頂点の色を補間してfrag側へ渡すよ

void main(void)
{
	gl_Position = ftransform();

	// ベクトルの準備
	vec3	P = (gl_ModelViewMatrix*gl_Vertex).xyz;		// 位置
	vec3	V = normalize(-P);				// 視線方向：物体→視点
	vec3	L = normalize(gl_LightSource[0].position.xyz);	// 入射方向：物体→光源
	vec3	N = normalize(gl_NormalMatrix*gl_Normal);	// 法線方向：物体→表側

	// 環境光成分
//	vec4	A = gl_FrontMaterial.ambient*gl_LightSource[0].ambient;
	vec4	A = gl_FrontLightProduct[0].ambient;

	// 拡散反射光成分：Lambertモデル
	float	d = max(dot(L, N), 0.0);
		// 光強度は非負のハズなのでmax()で範囲制限
//	vec4	D = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse;
	vec4	D = gl_FrontLightProduct[0].diffuse;
/*
	// 鏡面反射光成分：Phongモデル
	vec3	R = -normalize(reflect(L, N));		// 反射方向
	float	s = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);

	// ↑reflect()の計算が少々高コストなので...↓
*/
	// 鏡面反射光成分：Blinによる近似モデル
	vec3	H = normalize(V + L);	// ハーフベクトル
	float	s = pow(max(dot(H, N), 0.0), gl_FrontMaterial.shininess);

//	vec4	S = gl_FrontMaterial.specular*gl_LightSource[0].specular;
	vec4	S = gl_FrontLightProduct[0].specular;

	// 頂点の色
	color =	A + D*d + S*s*sign(d)
		+ gl_FrontLightModelProduct.sceneColor;
		// 陰影部分では鏡面反射もないハズなので*sign(d)で有無制御
		// LightModelはLightSource以外による環境光
}

