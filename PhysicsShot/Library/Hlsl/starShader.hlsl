
//global--------------------------------------
float4x4 _MatWorld : WORLD;
float4x4 _MatView : VIEW;
float4x4 _MatProj : PROJECTION;


//struct--------------------------------------
struct VS_OUT
{
	float4 pos : POSITION;
};



//fragmentShader------------------------------
VS_OUT VS(float4 pos : POSITION) : SV_POSITION
{
	VS_OUT outData;
	float4 outPos = pos;
	outPos = mul(outPos, _MatWorld);
	outPos = mul(outPos, _MatView);
	outPos = mul(outPos, _MatProj);
	outData.pos = outPos;

	return outData;
}


//pixelShader---------------------------------
half4 PS(VS_OUT inData) : COLOR
{
	
	return half4(1,1,0,1);
}

technique
{
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}