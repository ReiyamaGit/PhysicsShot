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
 *	@brief		�e�N�X�`���̏����擾����
 *	@remarks	�o�C���j�A��ԂŊ��炩�ɂ���
 ****************************************************/
sampler SamplerDecal = sampler_state
{
	Texture = <_TexDecal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*****************************************************
 *	@brief		NormalMap�̏����擾����
 *	@remarks	�o�C���j�A��ԂŊ��炩�ɂ���
 ****************************************************/
sampler SamplerNormal = sampler_state
{
	Texture = <_TexNormal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};



/*****************************************************
 *	@brief		���_���
 ****************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;	
	float4 eye      : TEXCOORD1;	
	float2 uv       : TEXCOORD0;	
	float4 light	: TEXCOORD2;	
};



/*****************************************************************
 *	@brief		���_�V�F�[�_�[�E
 *	@remarks	�\���̂ɃZ�}���e�B�N�X�����邩������̌��ɓ���Ȃ�
 ****************************************************************/
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0,float3 tangent : TEXCOORD1)
{
	VS_OUT outData;

	float4 outPos = pos;
	outPos = mul(outPos, _MatWorld);		// ���[���h�ϊ�
	outPos = mul(outPos, _MatView);		// �r���[�ϊ�
	outPos = mul(outPos, _MatProj);		// �ˉe�ϊ�	
	outData.pos = outPos;

	float3 binormal = cross(tangent, normal);

	normal = mul(float4(normal, 0), _MatRotate);		// �@��
	tangent = mul(float4(tangent, 0), _MatRotate);	// �ڐ�
	binormal = mul(float4(binormal, 0), _MatRotate);	// �]�@��

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
 *	@brief				�s�N�Z���V�F�[�_�[�E
 *	@param[inData]		���_���
 *	@remarks			���˂�n�C���C�g�̍Č�
 *	@return				�ύX��̖��邳Color���
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
 *	@brief				�V�F�[�_�[�̎w��
 ****************************************************************/
technique
{
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}