/******************************************************
 *	@file		Material.cpp
 *	@brief		Materialの実装
 *	@date		2017/02/14
 *	@name		hatakeyama
 ******************************************************/

//preprocessor------------------------------------------
#include "Material.h"
#include "ObjectPool.h"


/************************************************************
 *	@brief		コンストラクタ
 ************************************************************/
Material::Material()
	:m_shaderEffect(nullptr)
	,onShader(nullptr)
{
}


/************************************************************
 *	@brief		デストラクタ
 ************************************************************/
Material::~Material()
{
}




/***************************************************************************************
 *	@brief					Materialの作成
 *	@param[shaderName]		シェーダーのFile名
 *	@returns				Materialインスタンス
 *	@remarks				MaterialPoolに同じMaterialが存在した場合は,新しくは作成しない
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
 *	@brief					Materialの作成
 *	@param[shaderName]		シェーダーのFile名
 *	@returns				Materialインスタンス
 *	@remarks				MaterialPoolに同じMaterialが存在した場合は,新しくは作成しない
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
		MessageBox(NULL, "onShaderにラムダ式で関数を入れてください", "Shader error", MB_OK);
	}

}


