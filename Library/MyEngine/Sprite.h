
/******************************************************
 *	@file		Sprite.h
 *	@brief		画像を表示するクラス定義
 *	@data		2017/11/28
 *	@name		hatakeyama
 ******************************************************/

#pragma once

#include "Global.h"
#include "Node.h"



class Sprite: public Node
{
private:

	//D3DXSPRITEのポインタ
	LPD3DXSPRITE _pSprite;			//スプライト

	//DIRECT3DTEXTUREポインタ
	LPDIRECT3DTEXTURE9 _pTexture;	//テクスチャ

	RECT cut_;

	LPCSTR m_spriteName;

	D3DXVECTOR3 anchor_;


public:

	//コンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	//読み込み
	 virtual void Load(LPCSTR fileName,MyRect rect = MyRect(-999,-999,-999,-999));

	
	//描画
	 virtual void Draw() override;

	 void SetRect(MyRect rect);

	
	 void SetSprite(LPCSTR fileName,MyRect rect = MyRect(-1,-1,-1,-1));

	 D3DXMATRIX CreateWorldMatrix();


	 inline LPCSTR GetSpriteName(){ return m_spriteName; }
	
public:

	static Sprite* Create(LPCSTR fileName, MyRect rect);
	static Sprite* Create(LPCSTR fileName);
};

