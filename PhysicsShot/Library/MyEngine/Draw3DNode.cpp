/******************************************************
 *	@file		Draw3DNode.cpp
 *	@brief		Draw3DNode 実装
 *	@data		2017/12/04
 *	@name		hatakeyama
 ******************************************************/


//preprocessor--------------------------------
#include "Draw3DNode.h"



/******************************************************
 *	@brief		コンストラクタ
 ******************************************************/
Draw3DNode::Draw3DNode()
	:m_mesh(nullptr)
	, m_type(BOX)
	, m_radius(0)
	, m_boxData(0, 0, 0)
	, SLICE(52.0f)
	, STACK(52.0f)
	, m_material()
{
}




/******************************************************
 *	@brief		デストラクタ
 ******************************************************/
Draw3DNode::~Draw3DNode()
{
	R_SAFE_DELETE(m_mesh);
}




/******************************************************
 *	@brief		Draw3DNodeインスタンスを作成する
 *	@return		Draw3DNodeインスタンス
 ******************************************************/
Draw3DNode* Draw3DNode::Create()
{
	Draw3DNode* node = new Draw3DNode();

	if (node)
	{
		node->AutoRelease();
		return node;
	}

	R_SAFE_DELETE(node);


	return nullptr;
}




/******************************************************
*	@brief		球体の作成をするための情報を設定
******************************************************/
void Draw3DNode::CreateSphere(float radius, Color3D color)
{
	m_radius = radius;
	m_type = SPHERE;
	m_color = color;
}




/******************************************************
*	@brief		立方体を作成するための情報を設定
******************************************************/
void Draw3DNode::CreateBox(D3DXVECTOR3 v, Color3D color)
{
	m_boxData.x = v.x;
	m_boxData.y = v.y;
	m_boxData.z = v.z;
	m_type = BOX;
	m_color = color;
}



/**************************************************************
 *	@brief		描画する
 *	@remark		自分の子ノードがいる場合,NodeクラスのDrawも呼び,
 *				子ノードも描画する
 **************************************************************/
void Draw3DNode::Draw()
{

	if (m_transform->m_children.size() > 0)
	{
		Node::Draw();
	}


	switch (m_type)
	{
	case Draw3DNode::BOX:
		if (FAILED(D3DXCreateBox(GLOBAL::Global::m_device, m_boxData.x, m_boxData.y, m_boxData.z, &m_mesh, NULL)))
		{
			R_ASSERT(0);
		}
		break;
	case Draw3DNode::SPHERE:
		if (FAILED(D3DXCreateSphere(GLOBAL::Global::m_device, m_radius, SLICE, STACK, &m_mesh, NULL)))
		{
			R_ASSERT(0);
		}
		break;
	default:
		R_ASSERT(0);
		break;
	}


	D3DXMATRIX world = CreateWorldMatrix();

	GLOBAL::Global::m_device->SetTransform(D3DTS_WORLD, &world);

	CreateMaterial();




	m_mesh->DrawSubset(0);

	m_mesh->Release();
}




/******************************************************
 *	@brief		マテリアルの作成と適用
 ******************************************************/
void Draw3DNode::CreateMaterial()
{
	D3DMATERIAL9 mat;

	mat = D3DMATERIAL9();

	switch (m_color)
	{
	case Draw3DNode::RED:
		mat.Diffuse.r = 1;
		break;
	case Draw3DNode::BLUE:
		mat.Diffuse.b = 1;
		break;
	case Draw3DNode::GREEN:
		mat.Diffuse.g = 1;
		break;
	case Draw3DNode::BLACK:
		mat.Diffuse.r = 0;
		mat.Diffuse.g = 0;
		mat.Diffuse.b = 0;
		break;
	case Draw3DNode::WHITE:
		mat.Diffuse.r = 1.0f;
		mat.Diffuse.g = 1.0f;
		mat.Diffuse.b = 1.0f;
		break;
	default:
		break;
	}

	mat.Diffuse.a = 1.0f;

	GLOBAL::Global::m_device->SetMaterial(&mat);

	m_material = mat;
}




/******************************************************
 *	@brief		ワールド行列の作成
 *	@return		作成したワールド行列
 ******************************************************/
D3DXMATRIX Draw3DNode::CreateWorldMatrix()
{
	//移動行列

	D3DXMatrixTranslation(&m_transform->m_transMat, m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z);

	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateZ;

	//回転行列
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(m_transform->m_rotation.x));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(m_transform->m_rotation.y));
	D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(m_transform->m_rotation.z));

	m_transform->m_rotateMat = matRotateX * matRotateY * matRotateZ;

	D3DXMatrixScaling(&m_transform->m_scaleMat, m_transform->m_scale.x, m_transform->m_scale.y, m_transform->m_scale.z);



	D3DXMATRIX world = m_transform->m_scaleMat * m_transform->m_rotateMat * m_transform->m_transMat;



	return world;
}