// �O���[�o���ϐ�
// ���[���h�s��
float4x4 matWorld : WORLD;
// �r���[�s��
float4x4 matView : VIEW;
// �v���W�F�N�V�����s��
float4x4 matProj : PROJECTION;
// ��]�s��
float4x4 matRotate;
// �J�����̎��_�̈ʒu
float3 vecCam;
// ���C�g�̌���
float3 vecLightDir;
// �����Ή�����͈͂���ы��x
float aureolePower = 0.5f;

// �e�N�X�`���[
texture texDecal;

// �I�u�W�F�N�g�̃e�N�X�`���[
// sampler : �e�N�X�`���[�̏����܂Ƃ߂�����
//sampler s0 : register(s0);
// sampler : �e�N�X�`���[�̏����܂Ƃ߂�����
sampler SamplerDecal = sampler_state
{
	Texture = <texDecal>;

};

// ���_���
struct VS_OUT
{
	float4 pos		: SV_POSITION;	// �ύX��̈ʒu
	float4 normal	: NORMAL;
	float4 eye      : TEXCOORD1;	// �����x�N�g���F��p�̃e�b�N�X�R�[�h���Ȃ��ꍇTEXCOORD
	float2 uv       : TEXCOORD0;	// TEXCOORD0 = uv������
	float4 color	: COLOR;
	float  power : TEXCOORD2;	// ����̋��x
};

// ���_�V�F�[�_�[
// VS_OUT ���ɃZ�}���e�B�N�X�����邩������̌��ɃZ�}���e�B�N�X�͂���Ȃ� 
VS_OUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
	VS_OUT outData;

	// �ŏI�I�ɏo�͂���|�W�V����
	float4 outPos = pos;
	outPos = mul(outPos, matWorld);		// ���[���h�ϊ�
	outPos = mul(outPos, matView);		// �r���[�ϊ�
	outPos = mul(outPos, matProj);		// �ˉe�ϊ�	
	outData.pos = outPos;
	// �e���Œ肳��Ă���̂Ŗ@���ɉ�]�s�񂾂���������		���@����maya�ō�����Ƃ��̌����̂܂܂�����
	outData.normal = mul(float4(normal, 0), matRotate);

	outData.uv = uv;

	// �ύX��̒��_�̈ʒu
	float4 worldPos = mul(pos, matWorld);

	// ���_�x�N�g�������߂�
	outData.eye = normalize(float4(vecCam, 0) - worldPos);


	float4 light = float4(vecLightDir, 0);
	light = normalize(light);
	// �F
	outData.color = saturate(dot(outData.normal, -light));
	outData.color.a = 1;

	// ���C�g���J�����̐��ʂɗ���Ƌ����Ȃ�
	outData.power = max(0.0f,dot(outData.eye, vecLightDir));

	return outData;
}

// �s�N�Z���V�F�[�_�[
// ���_�V�F�[�_�[�̖߂�l������
float4 PS(VS_OUT inData) : COLOR
{
	// �I�u�W�F�N�g�̗֊s�t�߂̋��x�������Ȃ�悤�ɂ���
	float p = 1.0f - max(0.0f,dot(inData.normal, inData.eye));

	// �����Ή�����͈͂���ы��x�𒲐�����
	// ���̂�����̌v�Z�̓I�u�W�F�N�g�̃}�e���A���ɂ��C�ӂɕύX���邱��
	p = p * aureolePower;
	p = p * p;

	return inData.color * tex2D(SamplerDecal, inData.uv) + inData.power * p;

}

technique
{
	pass
	{
		// VS�֐��͒��_�V�F�[�_�[�Ɏw��
		VertexShader = compile vs_3_0 VS();
		// PS�֐��̓s�N�Z���V�F�[�_�[�Ɏw��
		PixelShader = compile ps_3_0 PS();
	}
}