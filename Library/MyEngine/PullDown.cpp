/********************************************************
 *	@file		PullDown.cpp
 *	@brief		PullDownListを作成するClass 実装
 *	@date		2018/03/21
 *	@name		hatakeyama
 *******************************************************/



//preprocessor----------------------------------
#include "PullDown.h"
#include "TouchListener.h"


/************************************************
 *	@brief			コンストラクタ
 ************************************************/
PullDown::PullDown()
: m_isDown(false)
{
	m_sprites.clear();
}


/************************************************
 *	@brief			デストラクタ
 ************************************************/
PullDown::~PullDown()
{
	m_sprites.shrink_to_fit();
}




/************************************************
 *	@brief				PullDownのインスタンス生成
 *	@param[sprites]		初期に追加する画像
 ************************************************/
PullDown* PullDown::Create(Sprite* sprites)
{
	PullDown* ret = new PullDown();



	if (ret)
	{
		ret->Init(sprites);
		ret->AutoRelease();
		return ret;
	}

	R_SAFE_DELETE(ret);


	return nullptr;
}




/********************************************
 *	@brief				PullDownリストに追加
 *	@param[sprite]		追加する画像
 ********************************************/
void PullDown::AddList(Sprite* sprite)
{
	sprite->m_transform->m_position = this->m_transform->m_position;
	sprite->m_transform->SetParent(this->m_transform);
	m_sprites.push_back(sprite);
	SetItemPosition();
}




/**********************************************************************************
 *	@brief				要素の削除
 *	@param[index]		削除する要素の番号
 *	@remarks			番号は0からスタート
 **********************************************************************************/
void PullDown::RemoveAtIndex(int index)
{

	R_SAFE_COUNT_RELEASE(m_sprites[index]);
	m_sprites.erase(m_sprites.begin() + index);
}



/**********************************************************************************
 *	@brief				初期化する
 *	@param[sprite]		最初に表示する画像
 *	@remarks			番号は0からスタート
 **********************************************************************************/
void PullDown::Init(Sprite* sprite)
{

	sprite->m_transform->m_position = this->m_transform->m_position;
	sprite->m_transform->SetParent(this->m_transform);
	m_sprites.push_back(sprite);

}





/**********************************************************************************
 *	@brief				初期化する
 *	@param[sprite]		最初に表示する画像
 *	@remarks			番号は0からスタート
 **********************************************************************************/
void PullDown::SetItemPosition()
{

	for (int i = 1; i < m_sprites.size(); i++)
	{
		m_sprites[i]->m_transform->m_position = D3DXVECTOR3(0, i * m_sprites[i - 1]->m_transform->m_size.y, 0);
	}
}




/**********************************************************************************
 *	@brief				入力処理 ※まだ未完成
 **********************************************************************************/
void PullDown::Input()
{
	
}





/**********************************************************************************
 *	@brief					画像を切り替える
 *	@param[mainSprite]		後から追加する画像
 **********************************************************************************/
void PullDown::MainSpriteChange(Sprite* mainSprite)
{

	for (int i = 0; i < m_sprites.size(); i++)
	{
		if (mainSprite->GetSpriteName() == m_sprites[i]->GetSpriteName())
		{
			m_sprites.erase(m_sprites.begin() + i);
		}
	}

	m_sprites.insert(m_sprites.begin(), mainSprite);
}