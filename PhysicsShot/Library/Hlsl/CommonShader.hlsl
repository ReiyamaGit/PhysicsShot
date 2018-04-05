
//Global-------------------------------------------------------
float4x4	_MatWorld : WORLD;			//ワールド行列
float4x4	_MatView  : VIEW;			//ビュー行列
float4x4	_MatProj  : PROJECTION;		//プロジェクション行列
float4x4	_MatRotate;					//回転行列
float4		_DiffuseColor;				//拡散反射光色
float4		_SpeculerColor;				//鏡面反射光色
float3		_VecCam;					//カメラの始点の位置
float3		_VecLightDir;				//ライトの向き
float		_SpeculerSize;				//鏡面反射のサイズ(鋭さ)
float		_HightParam = 0.016f;		//高さ最大値
texture		_TexDecal;					//テクスチャ
texture		_TexNormal;					//法線マップ(normalマップ)




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


/***************************************************
 *	@brief		normalMapを取得
 *	@remarks	バイリニア補間でなめらかにする
 ***************************************************/
sampler SamplerNormal = sampler_state
{
	Texture = <_TexNormal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/**************************************************
 *	@brief		頂点・視線・UV・lightの情報
 *  @remarks	専用のセマンティクスがない場合は,
 TEXCOORDを使う。
 **************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float4 eye      : TEXCOORD1;
	float2 uv       : TEXCOORD0;
	float4 light	: TEXCOORD2;
};

/***********************************************************************************************
 *	@brief			頂点シェーダー
 *	@param[pos]		座標
 *	@param[normal]	法線
 *	@param[uv]		UV座標
 *	@param[tangent]	タンジェント
 *  @remarks	セマンティクスがVS_OUT側にあるのでセマンティクスがいらない
 1.ワールド → ビュー → プロジェクションの順番に行列を適用する
 2.バイノーマル計算 →  バイノーマル(従法線) → 空間曲線上の1点で接触平面に垂直な直線
 3.法線と接線と従法線(以後3軸と呼ぶ)に回転行列をかける → そうじゃないとMayaまんまだから
 4.UV決定
 5.ワールド座標取得
 6.視線ベクトル計算
 ７．lightの設定。正規化し,3軸を向くようにする
 ************************************************************************************************/
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0, float3 tangent : TEXCOORD1)
{
	VS_OUT outData;

	//1
	float4 outPos = pos;
		outPos = mul(outPos, _MatWorld);
	outPos = mul(outPos, _MatView);
	outPos = mul(outPos, _MatProj);
	outData.pos = outPos;

	//2
	float3 binormal = cross(tangent, normal);

		//3
		normal = mul(float4(normal, 0), _MatRotate);
	tangent = mul(float4(tangent, 0), _MatRotate);
	binormal = mul(float4(binormal, 0), _MatRotate);

	//4
	outData.uv = uv;

	//5
	float4 worldPos = mul(pos, _MatWorld);

		//6.
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

/***************************************************************************
 *	@brief				ピクセルシェーダー
 *	@param[inData]		頂点情報
 *	@remarks			テクスチャなし＆法線マップなし
 シェーダーパスによる判断をする
 反射ベクトルは → ゲームをつくろーのサイトが参考になる
 光の強さを設定している
 *	@return				明るさ変更後の色
 **************************************************************************/
float4 PS(VS_OUT inData) : COLOR
{

	float3 normal = float3(0, 0, 1);

	float4 color;

	float4 NL = saturate(dot(float4(normal, 0), -inData.light));

		float4 ref = reflect(inData.light, float4(normal, 0));

		float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;

		color = NL * _DiffuseColor + speculer;

	color.a = 1;

	return color;
}

/******************************************************************
 *	@brief				ピクセルシェーダー
 *	@param[inData]		頂点情報
 *	@remark				テクスチャなし,法線マップあり
 *	@return				明るさ計算後の色
 ******************************************************************/
float4 PS_N(VS_OUT inData) : COLOR
{
	float4 color;

	float3 normal = tex2D(SamplerNormal, inData.uv).xyz * 2.0f - 1.0f;

		float4 NL = saturate(dot(float4(normal, 0), -inData.light));

		float4 ref = reflect(inData.light, float4(normal, 0));

		float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;

		color = NL * _DiffuseColor + speculer;

	color.a = 1;

	return color;
}

/******************************************************************
 *	@brief				ピクセルシェーダー
 *	@param[inData]		頂点情報
 *	@remark				テクスチャあり 法線マップなし
 *	@return				明るさ計算後の色
 ******************************************************************/
float4 PS_T(VS_OUT inData) : COLOR
{
	float3 normal = float3(0, 0, 1);

	float4 color;

	float4 NL = saturate(dot(float4(normal, 0), -inData.light));

		float4 ref = reflect(inData.light, float4(normal, 0));

		float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;
		color = NL * tex2D(SamplerDecal, inData.uv) + speculer;
	color.a = tex2D(SamplerDecal, inData.uv).a;

	return color;
}

/******************************************************************
 *	@brief				ピクセルシェーダー
 *	@param[inData]		頂点情報
 *	@remark				テクスチャあり 法線マップあり
 *	@return				明るさ計算後の色
 ******************************************************************/
float4 PS_TN(VS_OUT inData) : COLOR
{

	float h = tex2D(SamplerNormal, inData.uv).a;

	float2 tex = inData.uv + _HightParam * h * inData.eye.xy;

		float3 normal = tex2D(SamplerNormal, inData.uv).xyz * 2.0f - 1.0f;

		float4 color;

	float4 NL = saturate(dot(float4(normal, 0), -inData.light));

		float4 ref = reflect(inData.light, float4(normal, 0));

		float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;
		color = NL * tex2D(SamplerDecal, inData.uv) + speculer;
	color.a = tex2D(SamplerDecal, inData.uv).a;

	return color;
}



/******************************************************************
 *	@brief				シェーダーパスによって操作を分ける
 *	@param[pass]		Fbxからパスの数字を取得。これが判断材料
 *	@remarks
 1.テクスチャ		法線なし
 2.テクスチャ		法線なし
 3.テクスチャなし	法線あり
 4.テクスチャ		法線あり

 PS関数 → ピクセルシェーダー
 VS関数 → 頂点シェーダー
 ******************************************************************/
technique
{
	//1 
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
	//2
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_T();
	}
	//3
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_N();
	}
	//4
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_TN();
	}
}