//Global-------------------------------------------------------
float4x4 _MatWorld : WORLD;
float4x4 _MatView : VIEW;
float4x4 _MatProj : PROJECTION;
float4x4 _MatRotate;
float3 _VecCam;
float3 _VecLightDir;
texture _TexDecal;
texture _TexNormal;



/*****************************************************
 *	@brief		テクスチャの情報を取得する
 *	@remarks	バイリニア補間で滑らかにする
 ****************************************************/
sampler SamplerDecal = sampler_state
{
	Texture = <_TexDecal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*****************************************************
 *	@brief		NormalMapの情報を取得する
 *	@remarks	バイリニア補間で滑らかにする
 ****************************************************/
sampler SamplerNormal = sampler_state
{
	Texture = <_TexNormal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};



/*****************************************************
 *	@brief		頂点情報
 ****************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;	
	float4 eye      : TEXCOORD1;	
	float2 uv       : TEXCOORD0;	
	float4 light	: TEXCOORD2;	
};



/*****************************************************************
 *	@brief		頂点シェーダー・
 *	@remarks	構造体にセマンティクスがあるから引数の後ろに入らない
 ****************************************************************/
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0,float3 tangent : TEXCOORD1)
{
	VS_OUT outData;

	float4 outPos = pos;
	outPos = mul(outPos, _MatWorld);		// ワールド変換
	outPos = mul(outPos, _MatView);		// ビュー変換
	outPos = mul(outPos, _MatProj);		// 射影変換	
	outData.pos = outPos;

	float3 binormal = cross(tangent, normal);

	normal = mul(float4(normal, 0), _MatRotate);		// 法線
	tangent = mul(float4(tangent, 0), _MatRotate);	// 接線
	binormal = mul(float4(binormal, 0), _MatRotate);	// 従法線

	outData.uv = uv;

	float4 worldPos = mul(pos, _MatWorld);

	float3 eye = normalize(float4(_VecCam, 0) - worldPos);

	outData.eye.x = dot(eye, tangent);
	outData.eye.y = dot(eye, binormal);
	outData.eye.z = dot(eye, normal);
	outData.eye.w = 0;


	float4 light = float4(_VecLightDir, 0);
	light = normalize(light);
	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);
	outData.light.w = 0;
	


	return outData;
}



/*****************************************************************
 *	@brief				ピクセルシェーダー・
 *	@param[inData]		頂点情報
 *	@remarks			反射やハイライトの再現
 *	@return				変更後の明るさColor情報
 ****************************************************************/
float4 PS(VS_OUT inData) : COLOR
{
	float3 normal = tex2D(SamplerNormal, inData.uv).xyz * 2.0f -1.0f;

	float4 color;		

	float4 NL = saturate(dot(float4(normal,0), -inData.light));

	float4 ref = reflect(inData.light, float4(normal,0));
	float speculerSize = 3.0f;
	float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), speculerSize);
	color = NL * tex2D(SamplerDecal, inData.uv) + speculer;
	color.a = 1;

	return color;
}




/*****************************************************************
 *	@brief				シェーダーの指定
 ****************************************************************/
technique
{
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}