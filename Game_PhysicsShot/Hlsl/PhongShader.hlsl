//Global-------------------------------------------------------
float4x4 _MatWorld : WORLD;
float4x4 _MatView : VIEW;
float4x4 _MatProj : PROJECTION;
float4x4 _MatRotate;
float3 _VecCam;
float3 _VecLightDir;
//sampler s0 : register(s0); これはレジスタを確保するやつらしいがいつテクスチャを入れるのか
texture _MainTex;


sampler SamplerDecal = sampler_state
{
	Texture = <_MainTex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

/*****************************************************
 *	@brief		頂点情報	構造体
 ****************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;	// 変更後の位置
	float4 normal	: NORMAL;
	float4 eye      : TEXCOORD1;	// 視線ベクトル：専用のテックスコードがない場合TEXCOORD
	float2 tex		: TEXCOORD2;
};

/*****************************************************************
 *	@brief		頂点シェーダー・
 *	@remarks	構造体にセマンティクスがあるから引数の後ろに入らない
 ****************************************************************/
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL,float2 tex : TEXCOORD0)
{
	VS_OUT outData;

	float4 outPos = pos;
	outPos = mul(outPos, _MatWorld);		
	outPos = mul(outPos, _MatView);		
	outPos = mul(outPos, _MatProj);		
	outData.pos = outPos;
	outData.normal = mul(float4(normal,0), _MatRotate);

	outData.tex = tex;

	float4 worldPos = mul(pos, _MatWorld);

	outData.eye = normalize(float4(_VecCam,0) - worldPos);
	

	return outData;
}

/*****************************************************************
 *	@brief				ピクセルシェーダー・
 *	@param[inData]		頂点情報
 *	@remarks			反射やハイライトの再現
 *	@return				変更後の明るさColor情報
 ****************************************************************/
float4 PS_T(VS_OUT inData) : COLOR
{
	float4 color;		
	float4 light = float4(_VecLightDir, 0);
	light =	normalize(light);
	float4 NL = saturate(dot(inData.normal, -light));

	float4 ref = reflect(light, inData.normal);
	float speculerSize = 3.0f;
	float speculerPower = 2.0f;
	
	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), speculerSize);
	color = NL * tex2D(SamplerDecal, inData.tex) + speculer;
	color.a = 1;

	return color;
}


/*****************************************************
 *	@brief		シェーダーの指定をする
 ****************************************************/
technique
{
	pass
	{
		// VS関数は頂点シェーダーに指定
		VertexShader = compile vs_3_0 VS();
		// PS関数はピクセルシェーダーに指定
		PixelShader = compile ps_3_0 PS_T();
	}
}