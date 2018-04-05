/******************************************************
 *	@file		Material.cpp
 *	@brief		Material�̎���
 *	@date		2017/02/14
 *	@name		hatakeyama
 ******************************************************/

//preprocessor------------------------------------------
#include "Material.h"
#include "ObjectPool.h"


/************************************************************
 *	@brief		�R���X�g���N�^
 ************************************************************/
Material::Material()
	:m_shaderEffect(nullptr)
	,onShader(nullptr)
{
}


/************************************************************
 *	@brief		�f�X�g���N�^
 ************************************************************/
Material::~Material()
{
}




/***************************************************************************************
 *	@brief					Material�̍쐬
 *	@param[shaderName]		�V�F�[�_�[��File��
 *	@returns				Material�C���X�^���X
 *	@remarks				MaterialPool�ɓ���Material�����݂����ꍇ��,�V�����͍쐬���Ȃ�
 ***************************************************************************************/
Material* Material::Create()
{
	Material* ret = new Material();

	if (ret)
	{

		GLOBAL::Global::m_materialPool->AddObject(ret);
		return ret;

	}

	R_SAFE_DELETE(ret);


	return nullptr;
}



/***************************************************************************************
 *	@brief					Material�̍쐬
 *	@param[shaderName]		�V�F�[�_�[��File��
 *	@returns				Material�C���X�^���X
 *	@remarks				MaterialPool�ɓ���Material�����݂����ꍇ��,�V�����͍쐬���Ȃ�
 ***************************************************************************************/
void Material::SetShader(LPCSTR fileName)
{
	LPD3DXBUFFER errorLog;

	m_shaderName = fileName;

	if (FAILED(D3DXCreateEffectFromFile(GLOBAL::Global::m_device, fileName, NULL, NULL, NULL, NULL, &m_shaderEffect, &errorLog)))
	{
		MessageBox(NULL, (char*)errorLog->GetBufferPointer(), "Shader error", MB_OK);
	}


	if (onShader == nullptr)
	{
		MessageBox(NULL, "onShader�Ƀ����_���Ŋ֐������Ă�������", "Shader error", MB_OK);
	}

}


