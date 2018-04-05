/******************************************************
 *	@file		Quad.cpp
 *	@brief		Quad	実装
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/


//MyLibraryPreproccesor--------------------------------
#include "Quad.h"
#include "Camera.h"


/******************************************************
 *	@brief		コンストラクタ
 ******************************************************/
Quad::Quad()
{

	_pTexture = nullptr;
	_indexBuffer = nullptr;
	_vertexBuffer = nullptr;
}



/******************************************************
 *	@brief		デストラクタ
 ******************************************************/
Quad::~Quad()
{
	R_SAFE_RELEASE(_pTexture);
	R_SAFE_RELEASE(_vertexBuffer);
	R_SAFE_RELEASE(_indexBuffer);
	
}



/******************************************************
 *	@brief		Quadインスタンスを作成する
 *	@param		fileName	テクスチャファイルパス
 *	@return		作成されたQuadインスタンス
 ******************************************************/
Quad* Quad::Create(LPCSTR fileName)
{
	auto quad = new Quad();
	quad->Load(fileName);
	return quad;
}



/******************************************************
 *	@brief		Quadの作成に必要な情報を設定している
 *	@param		fileName	テクスチャファイルパス
 ******************************************************/
void Quad::Load(LPCSTR fileName)
{
	Vertex vertexList[] = {
		D3DXVECTOR3(-1, 1, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 0),
		D3DXVECTOR3(1, 1, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 0),
		D3DXVECTOR3(1, -1, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(1, 1),
		D3DXVECTOR3(-1, -1, 0), D3DXVECTOR3(0, 0, -1), D3DXVECTOR2(0, 1),

	};

	GLOBAL::Global::m_device->CreateVertexBuffer(sizeof(vertexList), 0, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, D3DPOOL_MANAGED, &_vertexBuffer, 0);

	Vertex *vCopy;
	_vertexBuffer->Lock(0, 0, (void**)&vCopy, 0);
	memcpy(vCopy, vertexList, sizeof(vertexList));
	_vertexBuffer->Unlock();


	int indexList[] = { 0, 2, 3, 0, 1, 2 };

	GLOBAL::Global::m_device->CreateIndexBuffer(sizeof(indexList), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_indexBuffer, 0);
	DWORD *iCopy;
	_indexBuffer->Lock(0, 0, (void**)&iCopy, 0);
	memcpy(iCopy, indexList, sizeof(indexList));
	_indexBuffer->Unlock();


	ZeroMemory(&_material, sizeof(D3DMATERIAL9));
	_material.Diffuse.r = 1.0f;
	_material.Diffuse.g = 1.0f;
	_material.Diffuse.b = 1.0f;


	D3DXCreateTextureFromFileEx(GLOBAL::Global::m_device, fileName, 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		NULL, NULL, NULL, &_pTexture);


}






/******************************************************
 *	@brief		描画をする
 *	@remarks	親ノードの可能性を考慮して,
				子を描画する関数を先頭で呼ぶ
 ******************************************************/
void Quad::Draw()
{
	Node::Draw();  //子を描画する関数

	GLOBAL::Global::m_device->SetStreamSource(0, _vertexBuffer, 0, sizeof(Vertex));
	GLOBAL::Global::m_device->SetIndices(_indexBuffer);
	GLOBAL::Global::m_device->SetMaterial(&_material);
	GLOBAL::Global::m_device->SetTexture(0, _pTexture);

	GLOBAL::Global::m_device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	//移動行列
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z);

	//回転行列
	D3DXMATRIX rotateX, rotateY, rotateZ;
	D3DXMatrixRotationX(&rotateX, D3DXToRadian(m_transform->m_rotation.x));
	D3DXMatrixRotationY(&rotateY, D3DXToRadian(m_transform->m_rotation.y));
	D3DXMatrixRotationZ(&rotateZ, D3DXToRadian(m_transform->m_rotation.z));

	//拡大縮小
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, m_transform->m_scale.z, m_transform->m_scale.y, m_transform->m_scale.z);

	

	//ワールド行列
	D3DXMATRIX world = scale * rotateZ * rotateX * rotateY * trans;

	if (GLOBAL::Global::m_mainCamera)
	{
		// ビルボード適用
		world = scale * GLOBAL::Global::m_mainCamera->GetBillBoard() * rotateZ * rotateX * rotateY * trans;
	}


	GLOBAL::Global::m_device->SetTransform(D3DTS_WORLD, &world);

	GLOBAL::Global::m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}