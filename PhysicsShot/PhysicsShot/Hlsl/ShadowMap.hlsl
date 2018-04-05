//Global-----------------------------------------
float4x4 _MatWorld;
float4x4 _MatView;
float4x4 _MatProj;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 world : TEXCOORD1;
};


//fragmentShader---------------------------------
VS_OUT VS(float4 pos : POSITION)
{
	VS_OUT outData;

	float4 outPos = pos;
	outPos = mul(outPos, _MatWorld);
	outPos = mul(outPos, _MatView);
	outPos = mul(outPos, _MatProj);
	outData.pos = outPos;

	outData.world = outPos;

	return outData;
}



//pixelShader-------------------------------------
float4 PS(VS_OUT inData) : COLOR
{
	float4 color = inData.world.z / inData.world.w;
	color.a = 1;
	return color;
}



technique
{
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}