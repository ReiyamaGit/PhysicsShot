//Global-------------------------------------------------------
float4x4 _MatWorld : WORLD;
float4x4 _MatView : VIEW;
float4x4 _MatProj : PROJECTION;
float4x4 _MatRotate;
float3 _VecCam;
float3 _VecLightDir;
texture _TexDecal;
texture _TexToon;			// �g�D�[���}�b�v�e�N�X�`���[
float4 _DiffuseColor;
float4 _SpeculerColor;
float _SpeculerSize;



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
 *	@brief		�g�D�[���e�N�X�`���̏����擾����
 *	@remarks	�o�C���j�A��ԂŊ��炩�ɂ���
 ****************************************************/
sampler SamplerToon = sampler_state
{
	Texture = <_TexToon>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


/*****************************************************
 *	@brief		���_���	�\����
 ****************************************************/
struct VS_OUT
{
	float4 pos		: SV_POSITION;	
	float4 eye      : TEXCOORD1;	
	float2 uv       : TEXCOORD0;	
	float4 light	: TEXCOORD2;	
};




/************************************************************************************
 *	@brief				���_�̏��̍X�V
 *	@param[pos]			�ʒu
 *	@param[normal]		�@�����
 *	@param[uv]			UV���W
 *	@param[tangent]		�^���W�F���g
 *	@remarks			���W�ϊ��Ə]�@���̍쐬�E�����x�N�g���̍쐬�E���C�g�̌�������
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

/************************************************************************************
 *	@brief				���_�̏��̍X�V
 *	@param[inData]		���_���
 *	@remark				�e�N�X�`���E�@���}�b�v�ǂ�����Ȃ�
 *	@return				���邳�Ƃ��ύX���Color���
 ***********************************************************************************/
float4 PS_T(VS_OUT inData) : COLOR
{
	float3 normal = float3(0, 0, 1);

	if (dot(inData.eye, normal) < 0.3f)
	{
		return float4(0, 0, 0, 1);
	}


	float4 NL = tex2D(SamplerToon, float2(saturate(dot(float4(normal, 0), -inData.light)), 0));

	float4 ref = reflect(inData.light, float4(normal, 0));

	float speculerPower = 2.0f;

	float4 speculer = speculerPower * pow(saturate(dot(ref, inData.eye)), _SpeculerSize) * _SpeculerColor;

		

	float4 color;		
	color = NL * tex2D(SamplerDecal, inData.uv);

	return color;
}



/************************************************************************************
 *	@brief				���W�ύX
 *	@param[pos]			�ʒu
 *	@param[normal]		�@��
 *	@return				�ϊ�����W
 ***********************************************************************************/
float4 VS_Outline(float4 pos : POSITION,float4 normal : NORMAL) : SV_POSITION
{
	normal.w = 0;

	float4 outPos = pos + normal * 0.01f;
	outPos = mul(outPos, _MatWorld);		
	outPos = mul(outPos, _MatView);		
	outPos = mul(outPos, _MatProj);			
	return outPos;
}


/************************************************************************************
 *	@brief				VS_Outline�ŏo�������W���������邱�ƂŃA�E�g���C����\������
 *	@param[pos]			���W
 *	@return				COLOR
 ***********************************************************************************/
float4 PS_Outline(float4 pos : SV_POSITION) : COLOR
{
	return float4(0, 0, 0, 1);
}




/***************************************************************************************
 *	@brief		�V�F�[�_�[�̌���
 *	@remark		
				1.�e�N�X�`������E�@���Ȃ� (�e���������肳����)
				2.�֊s�\��
 ***************************************************************************************/
technique
{
	pass
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS_T();
	}
	pass
	{
		VertexShader = compile vs_3_0 VS_Outline();
		PixelShader = compile ps_3_0 PS_Outline();
	}
}