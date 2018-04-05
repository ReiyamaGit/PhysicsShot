/******************************************************
 *	@file		Quad.cpp
 *	@brief		Quad	����
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/


//MyLibraryPreproccesor--------------------------------
#include "Quad.h"
#include "Camera.h"


/******************************************************
 *	@brief		�R���X�g���N�^
 ******************************************************/
Quad::Quad()
{

	_pTexture = nullptr;
	_indexBuffer = nullptr;
	_vertexBuffer = nullptr;
}



/******************************************************
 *	@brief		�f�X�g���N�^
 ******************************************************/
Quad::~Quad()
{
	R_SAFE_RELEASE(_pTexture);
	R_SAFE_RELEASE(_vertexBuffer);
	R_SAFE_RELEASE(_indexBuffer);
	
}



/******************************************************
 *	@brief		Quad�C���X�^���X���쐬����
 *	@param		fileName	�e�N�X�`���t�@�C���p�X
 *	@return		�쐬���ꂽQuad�C���X�^���X
 ******************************************************/
Quad* Quad::Create(LPCSTR fileName)
{
	auto quad = new Quad();
	quad->Load(fileName);
	return quad;
}



/******************************************************
 *	@brief		Quad�̍쐬�ɕK�v�ȏ���ݒ肵�Ă���
 *	@param		fileName	�e�N�X�`���t�@�C���p�X
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
 *	@brief		�`�������
 *	@remarks	�e�m�[�h�̉\�����l������,
				�q��`�悷��֐���擪�ŌĂ�
 ******************************************************/
void Quad::Draw()
{
	Node::Draw();  //�q��`�悷��֐�

	GLOBAL::Global::m_device->SetStreamSource(0, _vertexBuffer, 0, sizeof(Vertex));
	GLOBAL::Global::m_device->SetIndices(_indexBuffer);
	GLOBAL::Global::m_device->SetMaterial(&_material);
	GLOBAL::Global::m_device->SetTexture(0, _pTexture);

	GLOBAL::Global::m_device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	//�ړ��s��
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z);

	//��]�s��
	D3DXMATRIX rotateX, rotateY, rotateZ;
	D3DXMatrixRotationX(&rotateX, D3DXToRadian(m_transform->m_rotation.x));
	D3DXMatrixRotationY(&rotateY, D3DXToRadian(m_transform->m_rotation.y));
	D3DXMatrixRotationZ(&rotateZ, D3DXToRadian(m_transform->m_rotation.z));

	//�g��k��
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, m_transform->m_scale.z, m_transform->m_scale.y, m_transform->m_scale.z);

	

	//���[���h�s��
	D3DXMATRIX world = scale * rotateZ * rotateX * rotateY * trans;

	if (GLOBAL::Global::m_mainCamera)
	{
		// �r���{�[�h�K�p
		world = scale * GLOBAL::Global::m_mainCamera->GetBillBoard() * rotateZ * rotateX * rotateY * trans;
	}


	GLOBAL::Global::m_device->SetTransform(D3DTS_WORLD, &world);

	GLOBAL::Global::m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}