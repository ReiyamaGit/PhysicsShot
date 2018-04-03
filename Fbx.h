/******************************************************
 *	@file		Fbx.h
 *	@brief		Fbx 定義
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/


#ifndef __FBX__
#define __FBX__


//preprocessor--------------------------
#include "Node.h"
#include "Material.h"

//FBXSDK preprocessor-------------------
#include <fbxsdk.h>


//FBX SDK Library-----------------------
#pragma comment(lib,"libfbxsdk-mt.lib")


class PhysicsBody;

/**************************************
 *	@brief		レイキャストデータ構造体
 **************************************/
struct RayCastData
{
	D3DXVECTOR3 start;	//発射位置
	D3DXVECTOR3 dir;    //向き
	float		dist;	//衝突点までの距離
	BOOL		hit;	//レイが当たったか
};



/**************************************
 *	@brief		Fbx Class
 **************************************/
class Fbx :public Node
{

//struct--------------------------------
private:
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
		D3DXVECTOR3 tangent;
	};


//variable------------------------------
private:
	int		m_vertexCount;
	int		m_polygonCount;
	int		m_indexCount;
	int		m_materialCount;


	FbxManager*				 m_manager;
	FbxImporter*			 m_importer;
	FbxScene*				 m_scene;

	LPDIRECT3DVERTEXBUFFER9  m_vertexBuffer;



	D3DXMATRIX				matRotateX_;
	D3DXMATRIX				matRotateY_;
	D3DXMATRIX				matRotateZ_;

	Material*				m_materialData;

	PhysicsBody*			m_physicsBody;


//function-------------------------------------
private:
	void CheckNode(FbxNode* pNode);


	void CheckMesh(FbxMesh* pMesh);


	D3DXMATRIX CreateWorldMatrix();

	D3DXVECTOR3 CreateTan(D3DXVECTOR3 *pos0, D3DXVECTOR3 *pos1, D3DXVECTOR3 *pos2,
		D3DXVECTOR2 *uv0, D3DXVECTOR2 *uv1, D3DXVECTOR2 *uv2);
	
	

public:

	Fbx();

	~Fbx();

	static Fbx* Create(LPCSTR fileName);

	void Load(LPCSTR fileName);

	void Draw();

	void RayCast(RayCastData *data);

	void SetPhysicsBody(PhysicsBody* body);

	inline LPD3DXEFFECT GetShaderEffect() const { return m_materialData->m_shaderEffect; }

	PhysicsBody* GetPhysicsBody() { return m_physicsBody; }

	Material* GetMaterial() const { return m_materialData; }

};


#endif //!__FBX__

