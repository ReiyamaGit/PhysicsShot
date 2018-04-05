/******************************************************
 *	@file		Material.h
 *	@brief		Material�̒�`
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

	D3DMATERIAL9			m_material;					//FBX����擾����Material				->	Get material from FBX.

	LPDIRECT3DINDEXBUFFER9  m_indexBuffer;				//FBX����擾�����C���f�b�N�X			->	Get index from FBX.
	LPDIRECT3DTEXTURE9		m_texture;					//FBX����擾�����e�N�X�`��			->	Get texture from FBX.
	LPDIRECT3DTEXTURE9		m_normalTexture;			//FBX����擾�����m�[�}���e�N�X�`��		->	Get normalTexture from FBX.
	LPD3DXEFFECT			m_shaderEffect;				//�ݒ肳��Ă���V�F�[�_�[				->	Attached shader.
	LPCSTR					m_shaderName;				//�ݒ肳��Ă���V�F�[�_�[�t�@�C����	->	Attached shader file name.

	INT						m_polgonCountOfMaterial;	//�}�e���A�����Ƃ̃|���S����			->	Number of polygons per material.

	

public:
	typedef std::function<void()> OnShader;
	OnShader onShader;									//Shader���s���ɂ��Ȃ��Ă͂����Ȃ����Ƃ̋L�q

//function---------------------------------------------
public:
	Material();


	~Material();
	

	/***************************************************************************************
	 * @brief					�V�F�[�_�[�̒ǉ�
	 * @param[fileName]			�V�F�[�_�[�t�@�C����		
	 * @remarks					��̃}�e���A���Ɉ�̃V�F�[�_�[���t������_�ɒ���
	 *
	 * @remarks					��̃}�e���A���Ɉ�̃V�F�[�_�[���t������_�ɒ���
	 *							->	A single shader can be attached to one material.
	 ***************************************************************************************/
	void SetShader(LPCSTR fileName);


	/***************************************************************************************
	 * @brief					�C���X�^���X�쐬			->	Create Instance Of Material
	 * @param[shaderName]		�V�F�[�_�[�t�@�C����		->	File Name Of Shader
	 *
	 * @returns					�C���X�^���X				->	Return Created Instance
	 ***************************************************************************************/
	static Material* Create();


	friend class Fbx;
};


#endif //!__MATERIAL__
