//Global-------------------------------------------------------
float4x4		_MatWorld;					//world matrix
float4x4		_MatView;					//view matrix
float4x4		_MatProj;					//projection matrix
float4x4		_MatRotate;					//rotate  matrix
float4x4		_MatLightView;				//light view matrix
float4x4		_MatUV;						//uv matrix
float4			_DiffuseColor;				
float4			_SpeculerColor;
float3			_VecCam;					//Vector Camera
float3			_VecLightDir;				//light direction
texture			_TexDecal;					
texture			_TexShadowMap;				//shadowMap Texture
float			_SpeculerSize;
texture			_TexNormal;					//NormalMap
textureCUBE		_TexCube;					//CubeMap





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
 *	@brief		normalMapの情報を取得する
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
 *	@brief		CubeMapの情報を取得する
 *	@remarks	バイリニア補間で滑らかにする
 ****************************************************/
sampler SamplerCube = sampler_state
{
	Texture = <_TexCube>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*****************************************************
 *	@brief		ShadowMapに使うテクスチャを取得する
 *	@remarks	バイリニア補間で滑らかにする
 ****************************************************/
sampler SamplerShadowMap = sampler_state
{
	Texture = <_TexShadowMap>;

	MinFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*****************************************************
 *	@brief		頂点情報	構造体
 ****************************************************/
struct VS_OUT
{
	float4 pos			 : SV_POSITION;
	float2 uv			 : TEXCOORD0;
	float4 eye			 : TEXCOORD1;
	float4 light		 : TEXCOORD2;
	float4 lightTexCoord : TEXCOORD3;
	float4 lightViewPos  : TEXCOORD4;
};



/************************************************************************************
 *	@brief				頂点の情報の更新
 *	@param[pos]			位置
 *	@param[normal]		法線情報
 *	@param[uv]			UV座標
 *	@param[tangent]		タンジェント
 *	@remarks			座標変換と従法線の作成・視線ベクトルの作成・ライトの向き調整・ライトから見た位置の取得
 *	@return				変更後の頂点情報
 ***********************************************************************************/
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0, float3 tangent : TEXCOORD1)
{
	VS_OUT outData;

	float4 outPos = pos;
	outPos = mul(outPos, _MatWorld);
	outPos = mul(outPos, _MatView);
	outPos = mul(outPos, _MatProj);
	outData.pos = outPos;


	float3 binormal = cross(tangent, normal);
	normal = mul(float4(normal, 0), _MatRotate);
	tangent = mul(float4(tangent, 0), _MatRotate);
	binormal = mul(float4(binormal, 0), _MatRotate);

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

	outData.uv = uv;

	outData.lightViewPos = pos;
	outData.lightViewPos = mul(outData.lightViewPos, _MatWorld);
	outData.lightViewPos = mul(outData.lightViewPos, _MatLightView);
	outData.lightViewPos = mul(outData.lightViewPos, _MatProj);

	outData.lightTexCoord = mul(outData.lightViewPos, _MatUV);

	return outData;
}




/************************************************************************************
 *	@brief				頂点の情報の更新
 *	@param[inData]		頂点情報
 *	@remark				テクスチャ・法線マップどちらもなし。ライトからの距離を計算して光の減衰を適用する
 *	@return				明るさとか変更後のColor情報
 ***********************************************************************************/
float4 PS(VS_OUT inData) : COLOR
{
	float3 normal = float3(0, 0, 1);



	float4 NL = saturate(dot(float4(normal, 0), -inData.light));
	float4 ref = reflect(inData.light, float4(normal, 0));



	float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;

	float3 refEye = reflect(inData.eye, normal);
	float4 color = NL * _DiffuseColor + speculer + tex2D(SamplerCube, refEye) * 0.5f;
	color.a = 1;

	inData.lightTexCoord /= inData.lightTexCoord.w;
	float texValue = tex2D(SamplerShadowMap, inData.lightTexCoord).r;
	float atten = inData.lightViewPos.z / inData.lightViewPos.w;
	float bias = 0.003f;

	if (texValue < atten - bias)
	{
		color.rgb /= 3;
	}

	return color;
}


/************************************************************************************
 *	@brief				頂点の情報の更新
 *	@param[inData]		頂点情報
 *	@remark				テクスチャなし,法線マップあり
 *	@return				明るさとか変更後のColor情報
 ***********************************************************************************/
float4 PS_N(VS_OUT inData) : COLOR
{
	float3 normal = tex2D(SamplerNormal, inData.uv) * 2.0f - 1.0f;



	float4 NL = saturate(dot(float4(normal, 0), -inData.light));
	float4 ref = reflect(inData.light, float4(normal, 0));



	float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;

	float3 refEye = reflect(inData.eye, normal);
	float4 color = NL * _DiffuseColor + speculer + tex2D(SamplerCube, refEye) * 0.5f;
	color.a = 1;

	inData.lightTexCoord /= inData.lightTexCoord.w;
	float texValue = tex2D(SamplerShadowMap, inData.lightTexCoord).r;
	float atten = inData.lightViewPos.z / inData.lightViewPos.w;
	float bias = 0.003f;

	if (texValue < atten - bias)
	{
		color.rgb /= 3;
	}


	return color;
}


/************************************************************************************
 *	@brief				頂点の情報の更新
 *	@param[inData]		頂点情報
 *	@remark				テクスチャあり,,法線マップなし
 *	@return				明るさとか変更後のColor情報
 ***********************************************************************************/
float4 PS_T(VS_OUT inData) : COLOR
{
	float3 normal = float3(0, 0, 1);


	float4 NL = saturate(dot(float4(normal, 0), -inData.light));


	float4 ref = reflect(inData.light, float4(normal, 0));


	float speculerPower = 2.0f;


	float4 speculer = speculerPower *
		pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;


	float3 refEye = reflect(inData.eye, normal);
		float4 color = NL * tex2D(SamplerDecal, inData.uv) + speculer + tex2D(SamplerCube, refEye) * 0.5f;
		color.a = tex2D(SamplerDecal, inData.uv).a;

	inData.lightTexCoord /= inData.lightTexCoord.w;
	float texValue = tex2D(SamplerShadowMap, inData.lightTexCoord).r;
	float atten = inData.lightViewPos.z / inData.lightViewPos.w;
	float bias = 0.003f;

	if (texValue < atten - bias)
	{
		color.rgb /= 3;
	}

	return color;
}



/************************************************************************************
 *	@brief				頂点の情報の更新
 *	@param[inData]		頂点情報
 *	@remark				テクスチャ・法線マップどちらもあり
 *	@return				明るさとか変更後のColor情報
 ***********************************************************************************/
float4 PS_TN(VS_OUT inData) : COLOR
{
	float3 normal = tex2D(SamplerNormal, inData.uv) * 2.0f - 1.0f;


	float4 NL = saturate(dot(float4(normal, 0), -inData.light));
	
	
	float4 ref = reflect(inData.light, float4(normal, 0));


	float speculerPower = 2.0f;

	
	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;


	float3 refEye = reflect(inData.eye, normal);


	float4 color = NL * tex2D(SamplerDecal, inData.uv) + speculer + tex2D(SamplerCube, refEye) * 0.5f;

	
	color.a = tex2D(SamplerDecal, inData.uv).a;

	
	inData.lightTexCoord /= inData.lightTexCoord.w;
	
	
	float texValue = tex2D(SamplerShadowMap, inData.lightTexCoord).r;
	
	
	float atten = inData.lightViewPos.z / inData.lightViewPos.w;
	
	
	float bias = 0.003f;

	
	if (texValue < atten - bias)
	{
		color.rgb /= 3;
	}


	return color;
}




/***************************************************************************************
 *	@brief		シェーダーの決定
 *	@remark		fbxのシェーダーパスによって変更される
				1.テクスチャ・法線なし
			    2.テクスチャあり・法線なし
			    3.テクスチャなし・法線あり
			    4.テクスチャ・法線あり
 ***************************************************************************************/
technique
{
	//テクスチャも法線もなし
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}


	//テクスチャあり、法線なし
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_T();
	}


	//テクスチャなし、法線あり
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_N();
	}


	//テクスチャも法線あもり
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_TN();
	}
}