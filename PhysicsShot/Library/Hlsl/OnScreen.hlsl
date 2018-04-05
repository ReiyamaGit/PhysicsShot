/**********************************************************
 *	@file
 *	@brief
 *	@date
 *	@name
 **********************************************************/

texture _MainTex;




sampler SamplerDecal = sampler_state
{
	Texture = <_MainTex>;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float uv  : TEXCOORD0;
};


/******************************************
 *	@brief	fragmentShader
 ******************************************/
VS_OUT VS(float4 pos : SV_POSITION, float2 uv : TEXCOORD0)
{
	VS_OUT outData;
	outData.pos = pos;
	outData.uv = uv;


	return outData;

}




/******************************************
 *	@brief	pixelShader
 ******************************************/
float4 PS_T(VS_OUT inData)  : COLOR
{
	float4 color;
	color = tex2D(SamplerDecal, inData.uv);
	return color;
}

technique
{
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_T();
	}
}