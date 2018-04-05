//Global-------------------------------------------------------
float4x4 _MatWorld : WORLD;
float4x4 _MatView  : VIEW;
float4x4 _MatProj  : PROJECTION;
float4x4 _MatRotateX ;
float4x4 _MatRotateY ;
float4x4 _MatRotateZ ;


struct VS_OUT
{
	float4 pos	: SV_POSITION;
	float4 color : COLOR;
};



/*****************************************************
 *	@brief		頂点シェーダー,座標に行列を適用する
 *	@return		変換後座標
 ****************************************************/
VS_OUT VS(float4 pos : POSITION,float4 normal : NORMAL)
{
	
	VS_OUT outData;

	

	float4 outPos = pos;				
	float4 outNormal = normal;
	outPos = mul(outPos, _MatWorld);
	outPos = mul(outPos, _MatView);		
	outPos = mul(outPos, _MatProj);		
	outData.pos = outPos;
	normal = outNormal;

	float4 light = float4(-1, 1, -1, 0);
	light = normalize(light);
	outData.color = dot(normal, light);
	outData.color.a = 1;

	return outData;
}



/*****************************************************
 *	@brief		ピクセルシェーダー,座標に行列を適用する
 *	@return		変換後座標
 ****************************************************/
float4 PS(VS_OUT inData) : COLOR
{
	return inData.color/* * float4(1,1,1,1)*/;
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
		PixelShader = compile ps_3_0 PS();
	}
}