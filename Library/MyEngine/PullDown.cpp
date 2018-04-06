/********************************************************
 *	@file		PullDown.cpp
 *	@brief		PullDownListを作成するClass 実装
 *	@date		2018/03/21
 *	@name		hatakeyama
 *******************************************************/



 //preprocessor----------------------------------
#include "PullDown.h"
#include "TouchListener.h"



//const
const int FRAME_COUNT = 30;


/************************************************
 *	@brief			コンストラクタ
 ************************************************/
PullDown::PullDown()
	: m_isOpen(true)
	, m_currentIndex(0)
	, m_openButtonSize(0)
	, m_isAnimation(false)
	,m_mainSpriteName("")
{
	m_sprites.clear();
}


/************************************************
 *	@brief			デストラクタ
 ************************************************/
PullDown::~PullDown()
{
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
	m_openButtonSize = m_sprites[0]->m_transform->m_size.y * m_sprites.size();
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
	m_transform->m_children.erase(m_transform->m_children.begin() + index);
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
	m_mainSpriteName = m_sprites[0]->GetSpriteName();

	Input();

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


bool PullDown::Update(float delta)
{
	if (!Node::Update(delta))
	{
		return false;
	}

	if (m_isAnimation)
		ButtonAnimation();

	return true;
}


/**********************************************************************************
 *	@brief				入力処理 ※まだ未完成
 **********************************************************************************/
void PullDown::Input()
{
	//MainImageを押したとき
	//Animationを開始 → 終了後m_isOpenを反転

	TouchListener* listener = TouchListener::Create();

	listener->onTouchBegan = [this](TouchListener::Touch touch)
	{
		for (int i = 0; i < m_sprites.size(); i++)
		{
			if (m_sprites[i]->GetBoudingBox2D().ContainsPoint(GLOBAL::Global::m_input->GetMousePosition()))
			{
				m_mainSpriteName = m_sprites[i]->GetSpriteName();
				m_isAnimation = true;
				SwitchMainImage(true);
			}
		}

		return true;
	};

	GLOBAL::Global::m_eventDispatcher->AddEventListener(listener);
}





/**********************************************************************************
 *	@brief					画像を切り替える
 *	@param[mainSprite]		後から追加する画像
 **********************************************************************************/
void PullDown::SwitchMainImage(bool isClick)
{
	for (int i = 0; i < m_sprites.size(); i++)
	{
		if (!isClick && m_isOpen && m_mainSpriteName != m_sprites[i]->GetSpriteName())
		{
			m_sprites[i]->m_isActive = false;
		}
		else if(isClick)
		{
			m_sprites[i]->m_isActive = true;
		}

	}
	
}


void PullDown::ButtonAnimation()
{
	int open = m_isOpen ? -1 : 1;


	for (int i = 1; i < m_sprites.size(); i++)
	{

		if ((m_sprites[i]->m_transform->m_position.y > 0 && m_isOpen) 
		 || (m_sprites[i]->m_transform->m_position.y < m_sprites[i]->m_transform->m_size.y * i && !m_isOpen))
		{
			m_sprites[i]->m_transform->m_position = D3DXVECTOR3(m_sprites[i]->m_transform->m_position.x
				, m_sprites[i]->m_transform->m_position.y + m_sprites[i]->m_transform->m_size.y / FRAME_COUNT * i * open
				, m_sprites[i]->m_transform->m_position.z);
		}
		else if ((m_sprites[m_sprites.size() - 1]->m_transform->m_position.y <= 0 
			   || m_sprites[i]->m_transform->m_position.y >= m_sprites[i]->m_transform->m_size.y * i) 
			   && i == (m_sprites.size() - 1))
		{
			m_isAnimation = false;
			SwitchMainImage(false);
			m_isOpen = !m_isOpen;
		}
		
	}

}