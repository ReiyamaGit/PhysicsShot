// グローバル変数
// ワールド行列
float4x4 matWorld : WORLD;
// ビュー行列
float4x4 matView : VIEW;
// プロジェクション行列
float4x4 matProj : PROJECTION;
// 回転行列
float4x4 matRotate;
// カメラの視点の位置
float3 vecCam;
// ライトの向き
float3 vecLightDir;
// 後光を対応する範囲および強度
float aureolePower = 0.5f;

// テクスチャー
texture texDecal;

// オブジェクトのテクスチャー
// sampler : テクスチャーの情報をまとめたもの
//sampler s0 : register(s0);
// sampler : テクスチャーの情報をまとめたもの
sampler SamplerDecal = sampler_state
{
	Texture = <texDecal>;

};

// 頂点情報
struct VS_OUT
{
	float4 pos		: SV_POSITION;	// 変更後の位置
	float4 normal	: NORMAL;
	float4 eye      : TEXCOORD1;	// 視線ベクトル：専用のテックスコードがない場合TEXCOORD
	float2 uv       : TEXCOORD0;	// TEXCOORD0 = uv情報限定
	float4 color	: COLOR;
	float  power : TEXCOORD2;	// 後光の強度
};

// 頂点シェーダー
// VS_OUT 側にセマンティクスがあるから引数の後ろにセマンティクスはいらない 
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
	VS_OUT outData;

	// 最終的に出力するポジション
	float4 outPos = pos;
	outPos = mul(outPos, matWorld);		// ワールド変換
	outPos = mul(outPos, matView);		// ビュー変換
	outPos = mul(outPos, matProj);		// 射影変換	
	outData.pos = outPos;
	// 影が固定されているので法線に回転行列だけをかける		※法線はmayaで作ったときの向きのままだから
	outData.normal = mul(float4(normal, 0), matRotate);

	outData.uv = uv;

	// 変更後の頂点の位置
	float4 worldPos = mul(pos, matWorld);

	// 視点ベクトルを求める
	outData.eye = normalize(float4(vecCam, 0) - worldPos);


	float4 light = float4(vecLightDir, 0);
	light = normalize(light);
	// 色
	outData.color = saturate(dot(outData.normal, -light));
	outData.color.a = 1;

	// ライトがカメラの正面に来ると強くなる
	outData.power = max(0.0f,dot(outData.eye, vecLightDir));

	return outData;
}

// ピクセルシェーダー
// 頂点シェーダーの戻り値が引数
float4 PS(VS_OUT inData) : COLOR
{
	// オブジェクトの輪郭付近の強度が強くなるようにする
	float p = 1.0f - max(0.0f,dot(inData.normal, inData.eye));

	// 後光を対応する範囲および強度を調整する
	// このあたりの計算はオブジェクトのマテリアルにより任意に変更すること
	p = p * aureolePower;
	p = p * p;

	return inData.color * tex2D(SamplerDecal, inData.uv) + inData.power * p;

}

technique
{
	pass
	{
		// VS関数は頂点シェーダーに指定
		VertexShader = compile vs_3_0 VS();
		// PS関数はピクセルシェーダーに指定
		PixelShader = compile ps_3_0 PS();
	}
}