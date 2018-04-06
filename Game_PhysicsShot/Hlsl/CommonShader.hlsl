
//Global-------------------------------------------------------
float4x4	_MatWorld : WORLD;			//���[���h�s��
float4x4	_MatView  : VIEW;			//�r���[�s��
float4x4	_MatProj  : PROJECTION;		//�v���W�F�N�V�����s��
float4x4	_MatRotate;					//��]�s��
float4		_DiffuseColor;				//�g�U���ˌ��F
float4		_SpeculerColor;				//���ʔ��ˌ��F
float3		_VecCam;					//�J�����̎n�_�̈ʒu
float3		_VecLightDir;				//���C�g�̌���
float		_SpeculerSize;				//���ʔ��˂̃T�C�Y(�s��)
float		_HightParam = 0.016f;		//�����ő�l
texture		_TexDecal;					//�e�N�X�`��
texture		_TexNormal;					//�@���}�b�v(normal�}�b�v)




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


/***************************************************
 *	@brief		normalMap���擾
 *	@remarks	�o�C���j�A��ԂłȂ߂炩�ɂ���
 ***************************************************/
sampler SamplerNormal = sampler_state
{
	Texture = <_TexNormal>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/**************************************************
 *	@brief		���_�E�����EUV�Elight�̏��
 *  @remarks	��p�̃Z�}���e�B�N�X���Ȃ��ꍇ��,
 TEXCOORD���g���B
 **************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float4 eye      : TEXCOORD1;
	float2 uv       : TEXCOORD0;
	float4 light	: TEXCOORD2;
};

/***********************************************************************************************
 *	@brief			���_�V�F�[�_�[
 *	@param[pos]		���W
 *	@param[normal]	�@��
 *	@param[uv]		UV���W
 *	@param[tangent]	�^���W�F���g
 *  @remarks	�Z�}���e�B�N�X��VS_OUT���ɂ���̂ŃZ�}���e�B�N�X������Ȃ�
 1.���[���h �� �r���[ �� �v���W�F�N�V�����̏��Ԃɍs���K�p����
 2.�o�C�m�[�}���v�Z ��  �o�C�m�[�}��(�]�@��) �� ��ԋȐ����1�_�ŐڐG���ʂɐ����Ȓ���
 3.�@���Ɛڐ��Ə]�@��(�Ȍ�3���ƌĂ�)�ɉ�]�s��������� �� ��������Ȃ���Maya�܂�܂�����
 4.UV����
 5.���[���h���W�擾
 6.�����x�N�g���v�Z
 �V�Dlight�̐ݒ�B���K����,3���������悤�ɂ���
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
 *	@brief				�s�N�Z���V�F�[�_�[
 *	@param[inData]		���_���
 *	@remarks			�e�N�X�`���Ȃ����@���}�b�v�Ȃ�
 �V�F�[�_�[�p�X�ɂ�锻�f������
 ���˃x�N�g���� �� �Q�[��������[�̃T�C�g���Q�l�ɂȂ�
 ���̋�����ݒ肵�Ă���
 *	@return				���邳�ύX��̐F
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
 *	@brief				�s�N�Z���V�F�[�_�[
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`���Ȃ�,�@���}�b�v����
 *	@return				���邳�v�Z��̐F
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
 *	@brief				�s�N�Z���V�F�[�_�[
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`������ �@���}�b�v�Ȃ�
 *	@return				���邳�v�Z��̐F
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
 *	@brief				�s�N�Z���V�F�[�_�[
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`������ �@���}�b�v����
 *	@return				���邳�v�Z��̐F
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
 *	@brief				�V�F�[�_�[�p�X�ɂ���đ���𕪂���
 *	@param[pass]		Fbx����p�X�̐������擾�B���ꂪ���f�ޗ�
 *	@remarks
 1.�e�N�X�`��		�@���Ȃ�
 2.�e�N�X�`��		�@���Ȃ�
 3.�e�N�X�`���Ȃ�	�@������
 4.�e�N�X�`��		�@������

 PS�֐� �� �s�N�Z���V�F�[�_�[
 VS�֐� �� ���_�V�F�[�_�[
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