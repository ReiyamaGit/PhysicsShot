/****************************************
 *	@file		MyDebugDraw.cpp
 *	@brief		Debug表示をする際のクラス定義
 *	@date		2018/03/21
 *	@name		hatakeyama
 ****************************************/

//preprocessor--------------------------------
#include "MyDebugDraw.h"


/************************************************************
 *	@brief		コンストラクタ
 ************************************************************/
MyDebugDraw::MyDebugDraw()
{
}





/************************************************************
 *	@brief		デストラクタ
 ************************************************************/
MyDebugDraw::~MyDebugDraw()
{
}





/************************************************************
 *	@brief		線を描画する方法を指定
 ************************************************************/
void MyDebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

	GLOBAL::Global::m_device->CreateVertexBuffer(2 * sizeof(LineVertex),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_MANAGED, &vertexBuffer, NULL);

	LineVertex* v;
	vertexBuffer->Lock(0, 0, (VOID**)&v, 0);

	v[0].vector = from;
	v[1].vector = to;
	v[0].color = 0xffffffff;
	v[1].color = 0xffff0000;

	vertexBuffer->Unlock();

	D3DXMATRIX trans;
	D3DXMATRIX rotate;
	D3DXMATRIX scale;

	D3DXMatrixTranslation(&trans, 0, 0, 0);
	D3DXMatrixRotationX(&rotate, 0);
	D3DXMatrixScaling(&scale, 1, 1, 1);

	D3DXMATRIX world;
	world = scale * rotate * trans;


	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.a = 1.0f;
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 0.0f;
	material.Diffuse.b = 0.0f;

	GLOBAL::Global::m_device->SetStreamSource(0, vertexBuffer, 0, sizeof(LineVertex));
	GLOBAL::Global::m_device->SetFVF(D3DFVF_XYZ);
	GLOBAL::Global::m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	GLOBAL::Global::m_device->SetTexture(0, NULL);
	GLOBAL::Global::m_device->SetMaterial(&material);
	GLOBAL::Global::m_device->SetTransform(D3DTS_WORLD, &world);
	GLOBAL::Global::m_device->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	GLOBAL::Global::m_device->SetRenderState(D3DRS_LIGHTING, TRUE);

}