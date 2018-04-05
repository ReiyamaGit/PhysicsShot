
/******************************************************
 *	@file		Sprite.h
 *	@brief		�摜��\������N���X��`
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/

#pragma once

#include "Global.h"
#include "Node.h"



class Sprite: public Node
{
private:

	//D3DXSPRITE�̃|�C���^
	LPD3DXSPRITE _pSprite;			//�X�v���C�g

	//DIRECT3DTEXTURE�|�C���^
	LPDIRECT3DTEXTURE9 _pTexture;	//�e�N�X�`��

	RECT cut_;

	LPCSTR m_spriteName;

	D3DXVECTOR3 anchor_;


public:

	//�R���X�g���N�^
	Sprite();

	//�f�X�g���N�^
	~Sprite();

	//�ǂݍ���
	 virtual void Load(LPCSTR fileName,MyRect rect = MyRect(-999,-999,-999,-999));

	
	//�`��
	 virtual void Draw() override;

	 void SetRect(MyRect rect);

	
	 void SetSprite(LPCSTR fileName,MyRect rect = MyRect(-1,-1,-1,-1));

	 D3DXMATRIX CreateWorldMatrix();


	 inline LPCSTR GetSpriteName(){ return m_spriteName; }
	
public:

	static Sprite* Create(LPCSTR fileName, MyRect rect);
	static Sprite* Create(LPCSTR fileName);
};

