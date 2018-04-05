/******************************************************
 *	@file		Material.h
 *	@brief		Materialの定義
 *	@data		2017/02/14
 *	@name		hatakeyama
 ******************************************************/

#ifndef __MATERIAL__
#define __MATERIAL__

#include "Global.h"
#include "ObjectPool.h"
#include <functional>

//struct----------------------------------------------------------
typedef struct ShaderMatrix
{
	LPCSTR matrixName;
	D3DXMATRIX matrix;

	ShaderMatrix::ShaderMatrix(LPCSTR name, D3DXMATRIX mat)
	{
		matrixName = name;
		matrix = mat;
	}

};

typedef struct ShaderVector4
{
	LPCSTR vecName;
	D3DXVECTOR4 vector;

	ShaderVector4::ShaderVector4(LPCSTR name, D3DXVECTOR4 v)
	{
		vecName = name;
		vector = v;
	}
};



class Material
{

//variable---------------------------------------------
private:

	D3DMATERIAL9			m_material;					//FBXから取得したMaterial				->	Get material from FBX.

	LPDIRECT3DINDEXBUFFER9  m_indexBuffer;				//FBXから取得したインデックス			->	Get index from FBX.
	LPDIRECT3DTEXTURE9		m_texture;					//FBXから取得したテクスチャ			->	Get texture from FBX.
	LPDIRECT3DTEXTURE9		m_normalTexture;			//FBXから取得したノーマルテクスチャ		->	Get normalTexture from FBX.
	LPD3DXEFFECT			m_shaderEffect;				//設定されているシェーダー				->	Attached shader.
	LPCSTR					m_shaderName;				//設定されているシェーダーファイル名	->	Attached shader file name.

	INT						m_polgonCountOfMaterial;	//マテリアルごとのポリゴン数			->	Number of polygons per material.

	

public:
	typedef std::function<void()> OnShader;
	OnShader onShader;									//Shader実行時にしなくてはいけないことの記述

//function---------------------------------------------
public:
	Material();


	~Material();
	

	/***************************************************************************************
	 * @brief					シェーダーの追加
	 * @param[fileName]			シェーダーファイル名		
	 * @remarks					一つのマテリアルに一つのシェーダーが付属する点に注意
	 *
	 * @remarks					一つのマテリアルに一つのシェーダーが付属する点に注意
	 *							->	A single shader can be attached to one material.
	 ***************************************************************************************/
	void SetShader(LPCSTR fileName);


	/***************************************************************************************
	 * @brief					インスタンス作成			->	Create Instance Of Material
	 * @param[shaderName]		シェーダーファイル名		->	File Name Of Shader
	 *
	 * @returns					インスタンス				->	Return Created Instance
	 ***************************************************************************************/
	static Material* Create();


	friend class Fbx;
};


#endif //!__MATERIAL__
