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
 *	@brief		normalMap�̏����擾����
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
 *	@brief		CubeMap�̏����擾����
 *	@remarks	�o�C���j�A��ԂŊ��炩�ɂ���
 ****************************************************/
sampler SamplerCube = sampler_state
{
	Texture = <_TexCube>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*****************************************************
 *	@brief		ShadowMap�Ɏg���e�N�X�`�����擾����
 *	@remarks	�o�C���j�A��ԂŊ��炩�ɂ���
 ****************************************************/
sampler SamplerShadowMap = sampler_state
{
	Texture = <_TexShadowMap>;

	MinFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/*****************************************************
 *	@brief		���_���	�\����
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
 *	@brief				���_�̏��̍X�V
 *	@param[pos]			�ʒu
 *	@param[normal]		�@�����
 *	@param[uv]			UV���W
 *	@param[tangent]		�^���W�F���g
 *	@remarks			���W�ϊ��Ə]�@���̍쐬�E�����x�N�g���̍쐬�E���C�g�̌��������E���C�g���猩���ʒu�̎擾
 *	@return				�ύX��̒��_���
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
 *	@brief				���_�̏��̍X�V
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`���E�@���}�b�v�ǂ�����Ȃ��B���C�g����̋������v�Z���Č��̌�����K�p����
 *	@return				���邳�Ƃ��ύX���Color���
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
 *	@brief				���_�̏��̍X�V
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`���Ȃ�,�@���}�b�v����
 *	@return				���邳�Ƃ��ύX���Color���
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
 *	@brief				���_�̏��̍X�V
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`������,,�@���}�b�v�Ȃ�
 *	@return				���邳�Ƃ��ύX���Color���
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
 *	@brief				���_�̏��̍X�V
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`���E�@���}�b�v�ǂ��������
 *	@return				���邳�Ƃ��ύX���Color���
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
 *	@brief		�V�F�[�_�[�̌���
 *	@remark		fbx�̃V�F�[�_�[�p�X�ɂ���ĕύX�����
				1.�e�N�X�`���E�@���Ȃ�
			    2.�e�N�X�`������E�@���Ȃ�
			    3.�e�N�X�`���Ȃ��E�@������
			    4.�e�N�X�`���E�@������
 ***************************************************************************************/
technique
{
	//�e�N�X�`�����@�����Ȃ�
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}


	//�e�N�X�`������A�@���Ȃ�
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_T();
	}


	//�e�N�X�`���Ȃ��A�@������
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_N();
	}


	//�e�N�X�`�����@��������
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_TN();
	}
}