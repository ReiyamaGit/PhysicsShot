//Global-------------------------------------------------------
float4x4 _MatWorld : WORLD;
float4x4 _MatView : VIEW;
float4x4 _MatProj : PROJECTION;
float4x4 _MatRotate;
float3 _VecCam;
float3 _VecLightDir;
//sampler s0 : register(s0); ����̓��W�X�^���m�ۂ����炵�������e�N�X�`��������̂�
texture _MainTex;


sampler SamplerDecal = sampler_state
{
	Texture = <_MainTex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

/*****************************************************
 *	@brief		���_���	�\����
 ****************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;	// �ύX��̈ʒu
	float4 normal	: NORMAL;
	float4 eye      : TEXCOORD1;	// �����x�N�g���F��p�̃e�b�N�X�R�[�h���Ȃ��ꍇTEXCOORD
	float2 tex		: TEXCOORD2;
};

/*****************************************************************
 *	@brief		���_�V�F�[�_�[�E
 *	@remarks	�\���̂ɃZ�}���e�B�N�X�����邩������̌��ɓ���Ȃ�
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
 *	@brief				�s�N�Z���V�F�[�_�[�E
 *	@param[inData]		���_���
 *	@remarks			���˂�n�C���C�g�̍Č�
 *	@return				�ύX��̖��邳Color���
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
 *	@brief		�V�F�[�_�[�̎w�������
 ****************************************************/
technique
{
	pass
	{
		// VS�֐��͒��_�V�F�[�_�[�Ɏw��
		VertexShader = compile vs_3_0 VS();
		// PS�֐��̓s�N�Z���V�F�[�_�[�Ɏw��
		PixelShader = compile ps_3_0 PS_T();
	}
}