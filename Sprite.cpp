/******************************************************
 *	@file		Scheduler.cpp
 *	@brief		Scheduler	����
 *	@data		2017/12/05
 *	@name		hatakeyama
 ******************************************************/


//preprocessor-------------------------------
#include "Sprite.h"






/******************************************************
 *	@brief		�R���X�g���N�^
 ******************************************************/
Sprite::Sprite()
{
	_pSprite = nullptr;
	_pTexture = nullptr;
	m_spriteName = nullptr;
	cut_ = RECT{ 0, 0, 0, 0 };
	anchor_ = D3DXVECTOR3(0, 0, 0);
}





/******************************************************
 *	@brief		�f�X�g���N�^
 ******************************************************/
Sprite::~Sprite()
{
	R_SAFE_RELEASE(_pTexture);
	R_SAFE_RELEASE(_pSprite);

}






/******************************************************
 *	@brief				Sprite�C���X�^���X�쐬
 *	@param[fileName]	�t�@�C����
 *	@param[rect]		�؂���͈�
 *	@return				����:Sprite	���s:nullptr
 ******************************************************/
Sprite* Sprite::Create(LPCSTR fileName, MyRect rect)
{
	Sprite* sprite = new Sprite();

	if (sprite)
	{
		sprite->Load(fileName, rect);
		sprite->SetRect(rect);
		return sprite;
	}

	delete sprite;

	return nullptr;
}





/******************************************************
 *	@brief				Sprite�C���X�^���X�쐬
 *	@param[fileName]	�t�@�C����
 *	@return				����:Sprite	���s:nullptr
 ******************************************************/
Sprite* Sprite::Create(LPCSTR fileName)
{
	Sprite* sprite = new Sprite();

	if (sprite)
	{
		sprite->Load(fileName);
		return sprite;
	}

	delete sprite;

	return nullptr;
}


/******************************************************
 *	@brief				Sprite�̍쐬�ɕK�v�ȏ���ݒ�
 *	@param[fileName]	�t�@�C����
 *	@param[rect]		�؂���͈�
 ******************************************************/
void Sprite::Load(LPCSTR fileName, MyRect rect)
{
	m_spriteName = fileName;

	D3DXCreateSprite(GLOBAL::Global::m_device, &_pSprite);


	if (FAILED(D3DXCreateTextureFromFileEx(GLOBAL::Global::m_device, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &_pTexture)))
	{
		R_ASSERT(_pTexture);
	}




	if (rect.left == -999)
	{
		//�e�N�X�`���̃T�C�Y���擾
		D3DSURFACE_DESC d3dds;
		_pTexture->GetLevelDesc(0, &d3dds);
		m_transform->m_size.x = d3dds.Width;
		m_transform->m_size.y = d3dds.Height;

		m_rect = MyRect(0, 0, m_transform->m_size.x, m_transform->m_size.y);
	}
	else
	{
		SetRect(rect);
	}

}



/******************************************************
 *	@brief				�`�������
 ******************************************************/
void Sprite::Draw()
{

	Node::Draw();


	anchor_ = D3DXVECTOR3((m_rect.width - m_rect.left) * m_transform->m_anchorPoint.x, (m_rect.height - m_rect.top) * m_transform->m_anchorPoint.y, 0);


	cut_ = { (LONG)m_rect.left, (LONG)m_rect.top, (LONG)m_rect.width, (LONG)m_rect.height };

	


	_pSprite->SetTransform(&CreateWorldMatrix());

	_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	_pSprite->Draw(_pTexture, &cut_, &anchor_, nullptr, D3DCOLOR_ARGB(m_opacity, 255, 255, 255));

	_pSprite->End();
}





/******************************************************
 *	@brief				Rect����ݒ�
 *	@param[rect]		�؂���͈�
 ******************************************************/
void Sprite::SetRect(MyRect rect)
{
	m_rect = rect;
	m_transform->m_size.x = m_rect.width;
	m_transform->m_size.y = m_rect.height;
}



/******************************************************
 *	@brief				Sprite�̍Đݒ�
 *	@param[fileName]	�t�@�C����
 *	@param[rect]		�؂���͈�
 ******************************************************/
void Sprite::SetSprite(LPCSTR fileName, MyRect rect)
{
	
	m_spriteName = fileName;

	if (FAILED(D3DXCreateTextureFromFileEx(GLOBAL::Global::m_device, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &_pTexture)))
	{
		R_ASSERT(_pTexture);
	}


	if (rect.left == -1)
	{
		//�e�N�X�`���̃T�C�Y���擾
		D3DSURFACE_DESC d3dds;
		_pTexture->GetLevelDesc(0, &d3dds);
		m_transform->m_size.x = d3dds.Width;
		m_transform->m_size.y = d3dds.Height;

		m_rect = MyRect(0, 0, m_transform->m_size.x, m_transform->m_size.y);
	}
	else
	{
		SetRect(rect);
	}

}





/********************************************************
 *	@brief		World�s��̍쐬
 ********************************************************/
D3DXMATRIX Sprite::CreateWorldMatrix()
{

	
	D3DXMatrixTranslation(&m_transform->m_transMat, m_transform->m_parent->m_position.x + m_transform->m_position.x + GLOBAL::Global::WINDOW_WIDTH / 2.0f
						  ,m_transform->m_parent->m_position.y + m_transform->m_position.y + GLOBAL::Global::WINDOW_HEIGHT / 2.0f
						  , m_transform->m_parent->m_position.z + m_transform->m_position.z);


	D3DXMatrixRotationZ(&m_transform->m_rotateMat, D3DXToRadian(m_transform->m_parent->m_rotation.z + m_transform->m_rotation.z));

	D3DXMatrixScaling(&m_transform->m_scaleMat, m_transform->m_scale.x, m_transform->m_scale.y, 1);


	D3DXMATRIX parentWorld = m_transform->m_parent->m_scaleMat * m_transform->m_parent->m_rotateMat * m_transform->m_parent->m_transMat;

	D3DXMATRIX world = m_transform->m_scaleMat * m_transform->m_rotateMat * m_transform->m_transMat;

	return world * parentWorld;
}